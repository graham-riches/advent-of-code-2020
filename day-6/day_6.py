"""
    @file
    @brief
    @author
    @details
   
"""

# part one solution
inputs = [item.replace('\n', '') for item in open('day6.txt').read().split('\n\n')]
unique_set = [''.join(set(item)) for item in inputs]
total_letters = [len(item) for item in unique_set]
print(sum(total_letters))

# part two solution
inputs = [item.split('\n') for item in open('day6.txt').read().split('\n\n')]
common_elements = [len(set.intersection(*map(set, item))) for item in inputs]
print(sum(common_elements))
