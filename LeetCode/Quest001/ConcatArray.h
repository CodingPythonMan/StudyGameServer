#pragma once
#include <vector>

class ConcatArray {
public:
    std::vector<int> getConcatenation(std::vector<int>& nums) 
    {
        size_t size = nums.size();
        nums.reserve(size * 2);
        for (size_t i = 0; i < size; i++)
        {
            nums.push_back(nums[i]);
        }

        return nums;
    }
};