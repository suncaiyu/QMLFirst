#QMLFirst
注意到的事情：
  d当QML想使用在C ++中定义的内容，比如方法要用Q_INVOKABLE修饰，枚举要用Q_MENUMS修饰，但是当我在用信号中传递一个我定义的枚举值时，报了一个未定义的错误，当时我是使用Q_EMNUMS的，后来我导出成qRegisterMetaType <ImageProcessor :: Handletype>（“Handletype”）;就解决了，而且导导成这个之后，不写Q_EMNUMS没事，我不明白原理，现在只是单纯的记录   -2019/7/28
