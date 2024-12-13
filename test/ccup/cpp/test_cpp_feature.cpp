#include "catch2/catch.hpp"
#include <iostream>
#include <memory>
#include <vector>

class DataManager {
public:
    // 1. const 类属性需要构造初始化
    DataManager(const std::string& name, const std::vector<int>& data)
        : name_{name}, data_{data} {}

    // 2. const 成员函数返回 const 引用
    const std::string& getName() const {
        return name_;
    }

    // 3. 非 const 成员函数返回非常量引用
    std::vector<int>& getData() {
        return data_;
    }

    // 4. const 成员函数使用 mutable
    void logAccess() const {
        ++logCount_;
    }

    int getLogCount() const {
        return logCount_;
    }

private:
    const std::string name_;          // const 类属性
    std::vector<int> data_;           // 可修改数据
    mutable int logCount_{0};         // mutable 成员
};

TEST_CASE( "const usage" ) {
    // 创建 DataManager 对象
    std::vector<int> numbers = {10, 20, 30};
    DataManager manager("SensorA", numbers);

    // 访问 const 属性
    REQUIRE(manager.getName() == "SensorA");
    // manager.name_ = "SensorB"; // 编译错误

    // 修改数据
    manager.getData().push_back(40);
    REQUIRE(manager.getData().size() == 4);

    // 使用 const 成员函数修改 mutable
    manager.logAccess();
    manager.logAccess();
    REQUIRE(manager.getLogCount() == 2);
}

TEST_CASE("auto usage") {

    // 自动推导与初始化列表
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();       // std::vector<int>::iterator
    auto cit = vec.cbegin();     // std::vector<int>::const_iterator

    // 范围基 for 循环中的 auto
    for (auto& num : vec) {
        num += 1; // 修改原始 vector
    }

    // 使用 auto 避免冗长类型
    std::pair<std::string, int> p = {"Age", 30};
    auto [key, value] = p;        // 结构化绑定
    assert(key == "Age" && value == 30);

    // 使用 auto 与 lambda
    auto lambda = [](int a, int b) -> int { return a + b; };
    int sum = lambda(3, 4);
    assert(sum == 7);

    // 引用 和 const 推导
    int x = 5;
    int& ref = x;
    const int& constRef = x;

    auto d = ref;                // int (拷贝)
    auto& e = ref;               // int& 
    auto f = constRef;           // int (拷贝，忽略 const)
    auto& g = constRef;          // const int&

    // 指针 和 const 推导
    int* ptr = &x;
    const int* constPtr = &x;

    auto h = ptr;                // int* (拷贝)
    auto j = constPtr;           // const int* (拷贝，忽略 const)

}


int globalValue = 42;

// 返回类型为 decltype(auto)，根据返回表达式自动推导类型
decltype(auto) getGlobalValue() {
    int localVar = 10;
    int& refVar = localVar;
    const int constVar = localVar;
    const int& constRefVar = localVar;

    // 1. auto 的类型推导，忽略顶层 const 和引用
    auto a = localVar;       // int
    auto b = refVar;         // int
    auto c = constVar;       // int
    auto d = constRefVar;    // int

    // 2. decltype 的类型推导，保留 const 和引用
    decltype(localVar) e;             // int
    decltype(refVar) f = localVar;    // int&
    decltype(constVar) g = 20;        // const int
    decltype(constRefVar) h = localVar; // const int&

    // 3. decltype 不对表达式求值
    int x = 0;
    decltype(x++) i;    // i 的类型为 int，x 未被递增

    // 返回全局变量的引用
    return (globalValue);  // 使用括号使其返回 int& 类型
}

// // 类成员初始化顺序示例
// class Person {
// public:
//     // 有参构造隐藏了默认构造
//     // 单参数构造函数使用 explicit，防止隐式转换
//     explicit Person(const std::string& name) 
//     : name_(name) {
//     }

//     // 构造函数委托
//     Person(const std::string& name, int age) 
//     : Person(name), age_(age) {
//     }

//     // 不良构造，无效对象，重复赋值
//     Person() 
//     : name_("Unknown"), age_(0) {
//     }

//     // 区别构造和更新函数的使用场景
//     void setName(const std::string& name) {
//         name_ = name;
//     }

// private:
//     std::string name_;
//     int age_{0};  // 引起重复赋值
// };

class ResourceManager {
public:
    // 手动恢复默认构造函数
    ResourceManager() = default;

    // 析构函数默认是 noexcept 的
    ~ResourceManager() {
        // 释放资源
    }

    // 禁用拷贝构造函数和拷贝赋值运算符
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // 默认移动构造函数和移动赋值运算符，一般都是 noexcept 的
    ResourceManager(ResourceManager&&) noexcept = default;
    ResourceManager& operator=(ResourceManager&&) noexcept = default;

    // 交换函数确保不抛出异常，标记 noexcept
    void swap(ResourceManager& other) noexcept {
        // 交换资源
    }

    // 成员函数可能抛出异常
    void process() {
    }
};


void identifyValueCategory(int& lvalue) {
    std::cout << "lvalue\n";
}

void identifyValueCategory(int&& rvalue) {
    std::cout << "xvalue or prvalue\n";
}

TEST_CASE("value category") {
    int x = 10;
    int y = 20;

    // 1. lvalue
    int& lvalueRef = x;
    identifyValueCategory(x); // lvalue

    // 2. xvalue
    identifyValueCategory(std::move(x)); // xvalue or prvalue

    // 3. prvalue
    identifyValueCategory(30); // xvalue or prvalue

    int&& rvalueRef = 30;
    identifyValueCategory(rvalueRef); // lvalue
}

// void handleResource(const Resource& res) {
//     std::cout << "Handling lvalue Resource with data: " << res.data << "\n";
// }

// void handleResource(Resource&& res) {
//     std::cout << "Handling rvalue Resource with data: " << res.data << "\n";
// }

// template<typename T>
// void processResource(T&& res) {
//     // 完美转发，保持值类别
//     handleResource(std::forward<T>(res));
// }

// TEST_CASE("perfect forwarding") {
//     Resource res1{100};
//     processResource(res1); // lvalue
//     processResource(Resource{200}); // rvalue
//     processResource(std::move(res1)); // rvalue
// }


std::vector<int> createVectorWithRVO() {
    std::vector<int> vec = {100, 200, 300};
    return vec; // RVO 触发，直接在目标位置构造, 不用 move
}

TEST_CASE(" Test RVO and move semantics") {

    std::vector<int> vec = createVectorWithRVO();
    REQUIRE(vec.size() == 3);
    
    std::vector<int> vec1 = vec; // 拷贝构造

    REQUIRE(vec.size() == 3);
    REQUIRE(vec1.size() == 3);

    std::vector<int> vec2 = std::move(vec); // 移动构造

    REQUIRE(vec.size() == 0);
    REQUIRE(vec2.size() == 3);

    REQUIRE(vec1.size() == 3);

    std::vector<int> vec3 = createVectorWithRVO(); // RVO 触发，直接在目标位置构造, 不用 move
}

class MyVector {
    int* data_ {nullptr};
    size_t size_ {0};
public:
    // 构造函数
    MyVector(size_t size) 
    : data_{new int[size]}, size_{size} {
        for(size_t i = 0; i < size_; ++i) {
            data_[i] = 0;
        }
    }

    // 拷贝构造函数
    MyVector(const MyVector& other) 
    : size_{other.size_} {
        if(size_ > 0) {
            data_ = new int[size_];
            std::copy(other.data_, other.data_ + size_, data_);
        }
    }

    // 拷贝赋值运算符
    MyVector& operator=(const MyVector& other) {
        if(this == &other) {
            return *this; // 自我赋值检查
        }
        // 释放现有资源，分配新资源，拷贝数据
        if(data_) delete[] data_;

        size_ = other.size_;
        data_ = new int[size_];
        std::copy(other.data_, other.data_ + size_, data_);
        return *this;
    }

    // 移动构造函数
    MyVector(MyVector&& other) noexcept 
    : data_{other.data_}, size_{other.size_} {
        // 转移资源
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // 移动赋值运算符
    MyVector& operator=(MyVector&& other) noexcept {
        if(this == &other) {
            return *this; // 自我赋值检查
        }

        // 释放现有资源，转移资源
        if(data_) delete[] data_;
        data_ = other.data_;
        size_ = other.size_;

        other.data_ = nullptr;
        other.size_ = 0;

        return *this;
    }

    // 析构函数
    ~MyVector() {
        delete[] data_;
    }
};