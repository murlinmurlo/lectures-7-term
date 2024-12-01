#include "intrusive_ptr.h"

class TDoc : public TRef_counter<TDoc> {};

int main() {
    TIntrusivePtr<TDoc> ptr = nullptr;
    ptr = MakeIntrusive<TDoc>();
    TIntrusivePtr<TDoc> ptr2 = ptr;
    TIntrusivePtr<TDoc> ptr3 = MakeIntrusive<TDoc>();

    ptr3.Reset(ptr2);
    ptr3.Reset();
    ptr3.Reset(std::move(ptr2));

    static_assert(sizeof(TDoc*) == sizeof(ptr));

    return 0;
}