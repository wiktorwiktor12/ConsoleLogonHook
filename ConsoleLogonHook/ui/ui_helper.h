#pragma once
#define IM_VEC2_CLASS_EXTRA
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui_internal.h>

namespace ImGui
{
    static bool TextCustom(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size_arg = ImVec2(0, 0))
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
        ImGuiID id = window->GetID(label);
        ImVec2 label_size = CalcTextSize(label, NULL, true);
        ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
        ImVec2 pos = window->DC.CursorPos;
        pos.y += window->DC.CurrLineTextBaseOffset;
        ItemSize(size, 0.0f);

        // Fill horizontal space
        // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
        const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
        const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
        const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
        if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
            size.x = ImMax(label_size.x, max_x - min_x);

        // Text stays at the submission position, but bounding box may be extended on both sides
        const ImVec2 text_min = pos;
        const ImVec2 text_max(min_x + size.x, pos.y + size.y);

        // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
        ImRect bb(min_x, pos.y, text_max.x, text_max.y);
        if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
        {
            const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
            const float spacing_y = style.ItemSpacing.y;
            const float spacing_L = IM_TRUNC(spacing_x * 0.50f);
            const float spacing_U = IM_TRUNC(spacing_y * 0.50f);
            bb.Min.x -= spacing_L;
            bb.Min.y -= spacing_U;
            bb.Max.x += (spacing_x - spacing_L);
            bb.Max.y += (spacing_y - spacing_U);
        }
        //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

        // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackgroundChannel for every Selectable..
        const float backup_clip_rect_min_x = window->ClipRect.Min.x;
        const float backup_clip_rect_max_x = window->ClipRect.Max.x;
        if (span_all_columns)
        {
            window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
            window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
        }

        const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
        const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
        if (span_all_columns)
        {
            window->ClipRect.Min.x = backup_clip_rect_min_x;
            window->ClipRect.Max.x = backup_clip_rect_max_x;
        }

        if (!item_add)
            return false;

        const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
        if (disabled_item && !disabled_global) // Only testing this as an optimization
            BeginDisabled();

        // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
        // which would be advantageous since most selectable are not selected.
        if (span_all_columns)
        {
            if (g.CurrentTable)
                TablePushBackgroundChannel();
            else if (window->DC.CurrentColumns)
                PushColumnsBackground();
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasClipRect;
            g.LastItemData.ClipRect = window->ClipRect;
        }

        // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
        ImGuiButtonFlags button_flags = 0;
        if (flags & ImGuiSelectableFlags_NoHoldingActiveID) { button_flags |= ImGuiButtonFlags_NoHoldingActiveId; }
        if (flags & ImGuiSelectableFlags_NoSetKeyOwner) { button_flags |= ImGuiButtonFlags_NoSetKeyOwner; }
        if (flags & ImGuiSelectableFlags_SelectOnClick) { button_flags |= ImGuiButtonFlags_PressedOnClick; }
        if (flags & ImGuiSelectableFlags_SelectOnRelease) { button_flags |= ImGuiButtonFlags_PressedOnRelease; }
        if (flags & ImGuiSelectableFlags_AllowDoubleClick) { button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick; }
        if ((flags & ImGuiSelectableFlags_AllowOverlap) || (g.LastItemData.InFlags & ImGuiItemFlags_AllowOverlap)) { button_flags |= ImGuiButtonFlags_AllowOverlap; }

        const bool was_selected = selected;
        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

        // Auto-select when moved into
        // - This will be more fully fleshed in the range-select branch
        // - This is not exposed as it won't nicely work with some user side handling of shift/control
        // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
        //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
        //   - (2) usage will fail with clipped items
        //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
        if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == g.CurrentFocusScopeId)
            if (g.NavJustMovedToId == id)
                selected = pressed = true;

        // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
        if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
        {
            if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
            {
                SetNavID(id, window->DC.NavLayerCurrent, g.CurrentFocusScopeId, WindowRectAbsToRel(window, bb)); // (bb == NavRect)
                g.NavDisableHighlight = true;
            }
        }
        if (pressed)
            MarkItemEdited(id);

        // In this branch, Selectable() cannot toggle the selection so this will never trigger.
        if (selected != was_selected) //-V547
            g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

        // Render
        //if (hovered || selected)
        //{
        //    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
        //    RenderFrame(bb.Min, bb.Max, col, false, 0.0f);
        //}
        //if (g.NavId == id)
        //    RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_Compact | ImGuiNavHighlightFlags_NoRounding);

        if (span_all_columns)
        {
            if (g.CurrentTable)
                TablePopBackgroundChannel();
            else if (window->DC.CurrentColumns)
                PopColumnsBackground();
        }

        RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);

        // Automatically close popups
        if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.InFlags & ImGuiItemFlags_SelectableDontClosePopup))
            CloseCurrentPopup();

        if (disabled_item && !disabled_global)
            EndDisabled();

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
        return pressed; //-V1020
    }

    struct InputTextCallback_UserData
    {
        std::string* Str;
        ImGuiInputTextCallback  ChainCallback;
        void* ChainCallbackUserData;
    };
    
    static int InputTextCallback(ImGuiInputTextCallbackData* data)
    {
        InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
        if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
        {
            // Resize string callback
            // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
            std::string* str = user_data->Str;
            IM_ASSERT(data->Buf == str->c_str());
            str->resize(data->BufTextLen);
            data->Buf = (char*)str->c_str();
        }
        else if (user_data->ChainCallback)
        {
            // Forward to user callback, if any
            data->UserData = user_data->ChainCallbackUserData;
            return user_data->ChainCallback(data);
        }
        return 0;
    }
    
    static bool  InputText(const char* label, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;
    
        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }
    
    static bool  InputTextMultiline(const char* label, std::string* str, const ImVec2& size = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;
    
        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
    }
    
    static bool  InputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void* user_data = nullptr)
    {
        IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
        flags |= ImGuiInputTextFlags_CallbackResize;
    
        InputTextCallback_UserData cb_user_data;
        cb_user_data.Str = str;
        cb_user_data.ChainCallback = callback;
        cb_user_data.ChainCallbackUserData = user_data;
        return InputTextWithHint(label, hint, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
    }

    static void TextExNoAdd(const char* text, const char* text_end, ImGuiTextFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return;
        ImGuiContext& g = *GImGui;

        // Accept null ranges
        if (text == text_end)
            text = text_end = "";

        // Calculate length
        const char* text_begin = text;
        if (text_end == NULL)
            text_end = text + strlen(text); // FIXME-OPT

        const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
        const float wrap_pos_x = window->DC.TextWrapPos;
        const bool wrap_enabled = (wrap_pos_x >= 0.0f);
        if (text_end - text <= 2000 || wrap_enabled)
        {
            // Common case
            const float wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
            const ImVec2 text_size = CalcTextSize(text_begin, text_end, false, wrap_width);

            ImRect bb(text_pos, text_pos + text_size);
            ItemSize(text_size, 0.0f);
            //if (!ItemAdd(bb, 0))
            //    return;

            // Render (we don't hide text after ## in this end-user function)
            RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
        }
        else
        {
            // Long text!
            // Perform manual coarse clipping to optimize for long multi-line text
            // - From this point we will only compute the width of lines that are visible. Optimization only available when word-wrapping is disabled.
            // - We also don't vertically center the text within the line full height, which is unlikely to matter because we are likely the biggest and only item on the line.
            // - We use memchr(), pay attention that well optimized versions of those str/mem functions are much faster than a casually written loop.
            const char* line = text;
            const float line_height = GetTextLineHeight();
            ImVec2 text_size(0, 0);

            // Lines to skip (can't skip when logging text)
            ImVec2 pos = text_pos;
            if (!g.LogEnabled)
            {
                int lines_skippable = (int)((window->ClipRect.Min.y - text_pos.y) / line_height);
                if (lines_skippable > 0)
                {
                    int lines_skipped = 0;
                    while (line < text_end && lines_skipped < lines_skippable)
                    {
                        const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                        if (!line_end)
                            line_end = text_end;
                        if ((flags & ImGuiTextFlags_NoWidthForLargeClippedText) == 0)
                            text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                        line = line_end + 1;
                        lines_skipped++;
                    }
                    pos.y += lines_skipped * line_height;
                }
            }

            // Lines to render
            if (line < text_end)
            {
                ImRect line_rect(pos, pos + ImVec2(FLT_MAX, line_height));
                while (line < text_end)
                {
                    if (IsClippedEx(line_rect, 0))
                        break;

                    const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                    if (!line_end)
                        line_end = text_end;
                    text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                    RenderText(pos, line, line_end, false);
                    line = line_end + 1;
                    line_rect.Min.y += line_height;
                    line_rect.Max.y += line_height;
                    pos.y += line_height;
                }

                // Count remaining lines
                int lines_skipped = 0;
                while (line < text_end)
                {
                    const char* line_end = (const char*)memchr(line, '\n', text_end - line);
                    if (!line_end)
                        line_end = text_end;
                    if ((flags & ImGuiTextFlags_NoWidthForLargeClippedText) == 0)
                        text_size.x = ImMax(text_size.x, CalcTextSize(line, line_end).x);
                    line = line_end + 1;
                    lines_skipped++;
                }
                pos.y += lines_skipped * line_height;
            }
            text_size.y = (pos - text_pos).y;

            ImRect bb(text_pos, text_pos + text_size);
            ItemSize(text_size, 0.0f);
            //ItemAdd(bb, 0);
        }
    }

    static bool ImageButtonExCustom(ImGuiID id, const char* label, ImTextureID texture_id, const ImVec2& image_size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& bg_col, const ImVec4& tint_col, ImGuiButtonFlags flags)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        const ImVec2 padding = g.Style.FramePadding;
        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + image_size + padding * 2.0f);
        ItemSize(bb);
        if (!ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

        // Render
        const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        RenderNavHighlight(bb, id);
        RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
        if (bg_col.w > 0.0f)
            window->DrawList->AddRectFilled(bb.Min + padding, bb.Max - padding, GetColorU32(bg_col));
        window->DrawList->AddImage(texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, GetColorU32(tint_col));

        auto label_size = ImGui::CalcTextSize(label, NULL, true);

        const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + image_size.y + padding.y * 2.0f);
        ImRect bb2(text_pos, text_pos + label_size);
        //ItemSize(label_size, 0.0f);

        ImVec2 textmin = bb2.Min + padding;
        //textmin.y += image_size.y + padding.y * 2;
        ImVec2 textmax = bb2.Max - padding;
        //textmax.y += image_size.y + padding.y * 2;


        RenderTextClipped(textmin, textmax, label, NULL, &label_size, g.Style.SelectableTextAlign, &bb);

        return pressed;
    }

    static bool ImageButtonLabelled(const char* label, ImTextureID user_texture_id, const ImVec2& image_size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1))
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        if (window->SkipItems)
            return false;

        return ImageButtonExCustom(window->GetID(label), label, user_texture_id, image_size, uv0, uv1, bg_col, tint_col,0);
    }
}

static ImVec2 CalcTextButtonSize(std::string text)
{
    ImVec2 size;
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(text.c_str());
    const ImVec2 label_size = ImGui::CalcTextSize(text.c_str(), NULL, true);

    ImVec2 pos = window->DC.CursorPos;

    size = ImGui::CalcItemSize(size, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
    return size;
}

static bool ButtonCenteredOnLine(const char* label, const ImVec2& size = ImVec2(0, 0), float alignmentx = 0.5f, float widthOverride = 0.0f)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float sizex = (size.x <= 0 ? ImGui::CalcTextSize(label).x : size.x) + style.FramePadding.x * 2.0f;
    float availx = ImGui::GetContentRegionAvail().x;

    float offx = (availx - (widthOverride > 0 ? widthOverride : sizex)) * alignmentx;
    if (offx > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offx);

    return ImGui::Button(label, size);
}

static void ButtonCenteredOnLineNoCall(const char* label, const ImVec2& size = ImVec2(0, 0), float alignmentx = 0.5f, float widthOverride = 0.0f)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float sizex = (size.x <= 0 ? ImGui::CalcTextSize(label).x : size.x) + style.FramePadding.x * 2.0f;
    float availx = ImGui::GetContentRegionAvail().x;

    float offx = (availx - (widthOverride > 0 ? widthOverride : sizex)) * alignmentx;
    if (offx > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offx);

    
}


static void TextCenteredOnLine(const char* label, float alignmentx = 0.5f)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float sizex = ImGui::CalcTextSize(label).x;
    float availx = ImGui::GetContentRegionAvail().x;

    float offx = (availx - sizex) * alignmentx;
    if (offx > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offx);

    ImGui::Text(label);
}

static void CustomTextCenteredOnLine(const char* label, float alignmentx = 0.5f)
{
    ImGuiStyle& style = ImGui::GetStyle();

    float sizex = ImGui::CalcTextSize(label).x;
    float availx = ImGui::GetContentRegionAvail().x;

    float offx = (availx - sizex) * alignmentx;
    if (offx > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offx);

    ImGui::TextCustom(label);
}