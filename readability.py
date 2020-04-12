from cs50 import get_string

def main():
    user_in = get_string("Text: ")
    length = len(user_in)
    word_count = spaces(user_in)
    letter_count = letters(user_in)
    sentence_count = sentences(user_in)
    grade = ((0.0588 * ((letter_count / word_count) * 100))  - (0.296 * ((sentence_count / word_count) * 100)) - 15.8)
    
    if grade < 1:
        print("Before Grade 1")
    elif grade > 16:
        print("Grade 16+")
    else:
        print(f"Grade {round(grade)}")

# counts number of spaces in the text
def spaces(sent):
    count = 1
    for i in sent:
        if i == " ":
            count += 1
    #print("Spaces =", count)
    return(count)

# count letters
def letters(sent):
    letters = 0
    for i in sent:
        if i.isalpha() == True:
            letters += 1
    #print("Letters =", letters)
    return(letters)
    
# count number of sentences
def sentences(sent):
    sentences = 0
    for i in sent:
        if i in (".", "?", "!"):
            sentences += 1
    #print("Sentences =", sentences)
    return(sentences)
    
main()