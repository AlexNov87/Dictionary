#pragma once
#include <random>
class Rand
{
private:
    //std::uniform_real_distribution<> dis(0, max);
    std::random_device rand_device_;
    std::shared_ptr<std::mt19937_64> generator_;

public:
    Rand() {
       generator_ = std::make_shared<std::mt19937_64>(rand_device_());
    }
    
    std::shared_ptr<std::mt19937_64> GetGenerator() const {
        return generator_;
    }
   
};
