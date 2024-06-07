# DirectUI

`dui70.dll`，延续 mingkuang 的工作，主要探索了其制成成品软件的能力：

* `DirectUI::Value` 的各个 Type 。
* `IClassInfo`, `PropertyInfo`, `EnumMap` 构成的反射，并由此 dump 出 `/docs`，写 xml 查起来应该比较方便。
* 做了 `IEventListener` 的接口修正，和事件系统稍微一缝就能跑起来了。
  * 只看了 InputEvent，普通 Event 没看，估计是每种消息都有一种 Event，太麻烦了。
* 大致的 Layout 方法，看代码应该能看懂，光 `BorderLayout()` 就已经足够排出大部分自适应布局。
* style 的写法，`if` `unless` 是抄的 css 选择器，而 `ressheet` `resstr` 等资源相关的比较简单， duixml 中比较迷惑的是 `gtc` `gtf` 这些，实际上都是 uxtheme 的 API，想了解 ida 从 parser 找过去就可以了。
* 动画，虽然插值方法很多种，但属性比较少，所以只能支持简单的动画，看 animation enum 的取值即可。如果你有旧系统，可以把 Win7 或 Vista 的 "入门" 控制面板项中的 UIFILE 提取出来读，里面有伸缩位移+透明度混合的动画。
* Interop，由于底层是 Gdip，我想也没什么必要特地做；实际上有自己实现 IClassInfo，重写 Element 的虚方法，自制控件的办法，此时既可以用传统 WndProc 子类化，也可以弄个 Host 在自己里面继续套娃做 UserControl，就是写起来应该比较抽象。
* 兼容性，虽然 dui70 是个随系统 ship 的库，而且还有像是 ieui 等不少 fork，但它的兼容性似乎没什么问题，旧的接口从版本 8 一直到现在的版本 14 还是能用。

UxTheme 的时代曾还没开始就已经结束，Qt, yue 和 dui70 是三个仅存的火种。
