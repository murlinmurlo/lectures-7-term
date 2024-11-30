#include "intrusive_ptr.h"

class TDoc : public TRefCounter<TDoc> {
public:
    TDoc() {}
    ~TDoc() {}
};

int main() {
    TIntrusivePtr<TDoc> ptr = nullptr;
    ptr = MakeIntrusive<TDoc>();
    TIntrusivePtr<TDoc> ptr2 = ptr;
    TIntrusivePtr<TDoc> ptr3 = MakeIntrusive<TDoc>();
    
    ptr3.Reset(ptr2);
    ptr3.Reset();
    ptr3.Reset(std::move(ptr2));

    static_assert(sizeof(TDoc*) == sizeof(ptr));

    std::cout << ptr.RefCount() << std::endl;

    return 0;
}
