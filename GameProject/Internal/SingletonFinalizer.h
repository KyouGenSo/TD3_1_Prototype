#pragma once

/**
 * Res:mozc(https://github.com/google/mozc/blob/afb03ddfe72dde4cf2409863a3bfea160f7a66d8/src/base/singleton.h)
 *  Qiita(https://qiita.com/kikuuuty/items/fcf5f7df2f0493c437dc)
 */
class SingletonFinalizer{
public:
    using Finalizer = void(*)();
    static void AddFinalizer(Finalizer finalizer);
    static void Finalize();
};

