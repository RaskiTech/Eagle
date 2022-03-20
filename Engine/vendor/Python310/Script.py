def TemplateFunction():
    print("Hi from python template function!")

def ReturnFunction():
    return "The returned string"

def InputFunction(value):
    print("Took an input: " + value)

def MoreSophisticatedFunction(value):
    print("Got value " + value + " in this function")
    return value

if __name__ == "__main__":
    print("Running as main")
    TemplateFunction()
