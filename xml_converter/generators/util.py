from dataclasses import dataclass
from metadata import MetadataType


################################################################################
# lowercase
#
# A helper function to take each word in the string and convert it to a snake
# case string of lowercase letters. A delimiter can be passed in to change the
# characters inserted between the newly lowercased words.
################################################################################
def lowercase(word: str, delimiter: str = "_") -> str:
    wordarray = word.split(" ")
    lower_word_array = []

    for each_word in wordarray:
        lower_each_word = each_word.lower()
        lower_word_array.append(lower_each_word)

    return delimiter.join(lower_word_array)


################################################################################
# capitalize
#
# A helper function to take a snake case string and capitalize the first letter
# of each word in the string. A delimiter can be passed in to change the
# characters inserted between the newly capitalized words.
################################################################################
def capitalize(word: str, delimiter: str = " ") -> str:
    wordarray = word.split("_")
    capital_word_array = []

    for each_word in wordarray:
        capital_each_word = each_word.capitalize()
        capital_word_array.append(capital_each_word)

    return delimiter.join(capital_word_array)


################################################################################
# normalize
#
# A helper function to take a string and convert it to a string of
# lowercase letters.
################################################################################
def normalize(word: str) -> str:
    normalized_word_array = []

    for i, letter in enumerate(word):
        if letter.isupper():
            normalized_word_array.append(letter.lower())
        else:
            normalized_word_array.append(letter)

    return "".join(normalized_word_array)


@dataclass
class Document:
    metadata: MetadataType
    content: str
