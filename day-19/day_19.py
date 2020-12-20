"""
    @file day_19.py
    @brief day 19 solution. Had to cave and try Python because I suck at C++ too much
    @author Graham Riches
    @details
   
"""

class RuleParser:
    def __init__(self, rules: str) -> None:
        """
        create a new rules parser based on the passed in rules string
        :param rules: string of rules
        """
        self.rules = dict()
        for line in rules:
            key, value = self.parse_line(line)
            self.rules[key] = value

    @staticmethod
    def parse_line(line: str) -> tuple:
        """
        parse a line and return a key-value combination to go into the rules dictionary
        :param line: the line to parse
        :return: tuple of key/value pair
        """
        line = line.strip('\n')
        key_values = line.split(': ')
        key = int(key_values[0])
        raw_values = key_values[1]
        options = raw_values.split(' | ')

        values = list()
        for option in options:
            if 'a' in option:
                values.extend('a')
            elif 'b' in option:
                values.extend('b')
            else:
                items = option.split(' ')
                values.extend([[int(item) for item in items]])
        return key, values

    def get_all_combinations(self, key: int) -> list:
        """
        get all string combinations of a particular key
        :param key: the key
        :return:
        """
        if 'a' in self.rules[key]:
            return ['a']
        elif 'b' in self.rules[key]:
            return ['b']
        else:
            combinations = list()
            for rule in self.rules[key]:
                if len(rule) == 1:
                    values = self.get_all_combinations(rule[0])
                    combinations.extend(values)
                else:
                    a_values = self.get_all_combinations(rule[0])
                    b_values = self.get_all_combinations(rule[1])
                    com = [x + y for x in a_values for y in b_values]
                    combinations.extend(com)
        return combinations

    def count_instance(self, key: int, lines: list) -> int:
        """
        count how many times a key occurs in a list of lines
        :param key: the key to check
        :param lines: list of lines
        :return: count
        """
        combinations = self.get_all_combinations(key)
        count = 0
        for line in lines:
            line = line.strip('\n')
            if line in combinations:
                count += 1
        return count


if __name__ == '__main__':
    rules_data = open('data/rules2.txt').readlines()
    data = open('data/input.txt').readlines()
    parser = RuleParser(rules_data)
    total = parser.count_instance(0, data)
    print('Total count {}'.format(total))
