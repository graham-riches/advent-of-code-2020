"""
    @file day_23.py
    @brief day 23 python version solution
    @author Graham Riches
    @details
    quick, terribly non-performant and inefficient solution for part 1 of day 23. As part two requires a massive
    linked list solution, the C++ version will be WAY faster, and should be more fun to work on :D
"""


class CircularList:
    def __init__(self, entries: list) -> None:
        """
        create a new circular list object
        :param entries: default list entries
        """
        self.entries = entries
        self.list_size = len(entries)
        self.min_entry = min(entries)
        self.max_entry = max(entries)

    @property
    def length(self) -> int:
        return self.list_size

    @property
    def min(self) -> int:
        return self.min_entry

    @property
    def max(self) -> int:
        return self.max_entry

    def __getitem__(self, index):
        """
        get an item from the container by index
        :param index: index
        :return:
        """
        return self.entries[index]

    def pop_entries_from_index(self, index: int, count: int = 3) -> list:
        """
        pop a number of items from a circular list starting AFTER the passed in index
        :param index: the index to pop from
        :param count: how many items to pop
        :return: sub list
        """
        popped_list = list()
        if index + count < self.list_size:
            popped_list = self.entries[index + 1: index + count + 1]
            del self.entries[index + 1: index + count + 1]
        else:
            back_cnt = self.list_size - (index + 1)
            if back_cnt > 0:
                popped_list = self.entries[-back_cnt:]
                popped_list.extend(self.entries[0:count - back_cnt])
                del self.entries[-back_cnt:]
                del self.entries[0:count - back_cnt]
            else:
                popped_list = self.entries[0:count]
                del self.entries[0:count]
        self.list_size -= count
        return popped_list

    def insert_at_index(self, index: int, items: list) -> None:
        """
        insert items into the list at a specific index
        :param index: index to insert at
        :param items: items to insert
        :return: None
        """
        for i in range(len(items)):
            self.entries.insert(index + i, items[i])
        self.list_size += len(items)

    def find_index_by_entry(self, entry: int) -> int:
        """
        find an index by the entry value
        :param entry:
        :return: the index of that cup
        """
        return self.entries.index(entry, 0, self.list_size)


class CrabbyCupsGame:
    def __init__(self, cups: CircularList) -> None:
        """
        create a new crabby cups game with a set number of rounds
        :param cups: circular list object of game cups
        """
        self.cups = cups

    def find_destination_index(self, active_cup: int) -> int:
        """
        get the destination cup index from the active cup
        :param active_cup:
        :return:
        """
        target_cup_id = active_cup - 1
        while True:
            try:
                destination_index = self.cups.find_index_by_entry(target_cup_id)
                return destination_index
            except ValueError as e:
                pass
            target_cup_id -= 1
            if target_cup_id < self.cups.min:
                target_cup_id = self.cups.max

    def play_game(self, rounds: int) -> CircularList:
        """
        play the game for a set number of rounds and return the cups afterwards
        :param rounds: how many rounds
        :return: cups after game is played
        """
        active_cup = self.cups[0]
        for round in range(rounds):
            idx = self.cups.find_index_by_entry(active_cup)
            round_cups = self.cups.pop_entries_from_index(idx)
            destination = self.find_destination_index(active_cup)
            self.cups.insert_at_index(destination + 1, round_cups)
            active_cup_index = (self.cups.find_index_by_entry(active_cup) + 1) % self.cups.length
            active_cup = self.cups[active_cup_index]
        return self.cups


if __name__ == '__main__':
    initial = [3, 8, 9, 1, 2, 5, 4, 6, 7]
    game = CrabbyCupsGame(CircularList(initial))
    cups = game.play_game(100)
    idx = cups.find_index_by_entry(1)
    order = cups[idx:]
    order.extend(cups[0:idx])
    print(''.join('{}'.format(x) for x in order[1:]))



