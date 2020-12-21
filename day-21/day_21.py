"""
    @file
    @brief
    @author
    @details
   
"""
import collections


if __name__ == '__main__':
    # Read the input, and create a map of allergens to possible ingredients as well as a total ingredient count
    allergen_map = collections.defaultdict(list)
    ingredient_count = collections.Counter()
    for line in open('input.txt').readlines():
        components = line.strip(')\n').split(' (contains ')
        ingredients = components[0].split(' ')
        allergens = components[1].strip(')').split(', ')
        for ingredient in ingredients:
            ingredient_count[ingredient] += 1
        for allergen in allergens:
            allergen_map[allergen].append(set(ingredients))

    # Part One: separate out the allergens using the intersection of the lists from the prior dict
    allergen_dict = {key: set.intersection(*values) for key, values in allergen_map.items()}
    allergens = set.union(*allergen_dict.values())
    safe_foods = [ingredient for ingredient, value in ingredient_count.items() if ingredient not in allergens]
    safe_food_count = [value for key, value in ingredient_count.items() if key in safe_foods]
    print('Safe food count: {}'.format(sum(safe_food_count)))

    # Part Two: associate each allergen with it's alphabetical name (e.g. dairy)
    allergen_type = dict()
    allergens_set = set()
    while len(allergens_set) != len(allergens):
        # get a sorted list of allergens by the length of possible types. The first element should always have length
        # of one, which corresponds to a match to a specific ingredient type
        sorted_allergens = sorted(allergen_dict.items(), key=lambda item: len(item[1]))
        item = sorted_allergens[0]

        # get the logic not of the current allergens the existing set
        new_allergen = {s for s in item[1] if s not in allergens_set}

        # update the existing set with the new value
        allergens_set = set.union(allergens_set, new_allergen)

        # stuff the ingredient/allergen combo into the new dictionary
        allergen_strs = [str(allergen) for allergen in new_allergen]
        allergen_type[allergen_strs[0]] = item[0]

        # remove the current allergen from every other entry so that the list can be re-sorted
        new_dict = dict()
        for other in sorted_allergens:
            if other != item:
                new_dict[other[0]] = set.symmetric_difference(other[1], new_allergen)
        allergen_dict = new_dict

    # sort the allergen/name dict by name and pack it into a string
    alphabetic_sort = sorted(allergen_type.items(), key=lambda item: item[1])
    print(','.join(item[0] for item in alphabetic_sort))
