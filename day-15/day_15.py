"""
    @file
    @brief
    @author
    @details
   
"""

inputs = [int(item) for item in open('puzzle.txt').read().split(',')]


class PriorityList:
    def __init__(self, max_length: int = 2) -> None:
        """
        construct a priority list with a max length
        :param max_length: max length of the list
        """
        self.data = list()
        self.queue_len = max_length

    def insert(self, item: int) -> None:
        """
        insert an item on the list. This maintains the list size by popping a value off the front
        :param item: the item to insert
        :return: none
        """
        self.data.append(item)
        if len(self.data) > self.queue_len:
            self.data.pop(0)

    def get_delta(self, index_one: int = -1, index_two: int = -2) -> int:
        """
        return the delta between two list items
        :param index_one: first index
        :param index_two: second index
        :return: difference
        """
        return self.data[index_one] - self.data[index_two]

    def is_full(self) -> bool:
        """
        returns true if the list is at it's max size
        :return:
        """
        return len(self.data) == self.queue_len


def get_turn_value(n: int, input: list) -> int:
    """
    get the game turn score at iteration n given the input list input
    :param n: turn score iteration
    :param input: the input game data
    :return: int value
    """
    locations = dict()
    for idx, value in enumerate(input):
        if value in locations.keys():
            locations[value].insert(idx)
        else:
            locations[value] = PriorityList(2)
            locations[value].insert(idx)

    check_val = input[-1]
    for i in range(len(input), n):
        new_score = 0
        if check_val in locations.keys():
            if locations[check_val].is_full():
                new_score = locations[check_val].get_delta()

        if new_score in locations.keys():
            locations[new_score].insert(i)
        else:
            locations[new_score] = PriorityList(2)
            locations[new_score].insert(i)
        check_val = new_score
    return check_val


a = get_turn_value(30000000, inputs)
print(a)

