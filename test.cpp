//
// Created by 赵鲲翔 on 2024/9/27.
//
#include <vector>
#include <set>
#include "gtest/gtest.h"
using namespace std;

void updates(vector<long long > &arr,long long k,long long x
        ,long long y,long long c,long long p){
    for (auto i=arr.begin();i!=arr.end();i++){
        *i=((x*x+k*y+5*x)%p)*c;
    }

};
long long querys(vector<long long > &arr){
    long long sum=0;
    for (auto i=arr.begin();i!=arr.end();i++){
        sum+=*i;
    }
    return sum;
};
long long countDistincts(vector<long long > &arr){
    set<long long > a;
    for(auto elemet:arr){
        auto b=a.insert(elemet);
        if (b.second==false){
            a.insert(-elemet);
        }
    }
    return a.size();
}

class ArrayTest : public testing::Test {
protected:
    void SetUp() override {
        arr={1,2,3,4,5};
        arr2={-1,1,1,2,2};
        arr3={0,1,-1,2,-2};

    }
    void TearDown() override {
        arr.clear();
    }
    vector<long long > arr,arr2,arr3;
};

TEST_F(ArrayTest, updateTest){
    EXPECT_EQ(countDistincts(arr2),4);
    EXPECT_EQ(countDistincts(arr3),5);
}

TEST_F(ArrayTest, queryTest){
    EXPECT_EQ(querys(arr),15);
    EXPECT_EQ(querys(arr2),5);
    EXPECT_EQ(querys(arr3),0);
}

TEST_F(ArrayTest, countDistinctTest){

    EXPECT_EQ(countDistincts(arr),5);

}


// 是不是路径不能重叠？ ，出现问题，如果include main.cpp
// 分为两个可执行文件


