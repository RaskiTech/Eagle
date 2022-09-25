print("Start reading start of file. __name__ is " + __name__)

def TemplateFunction(value):
    print("Hi from a python function! Took input: " + str(value))
    return "Return value"

def Another(value):
    pass

if __name__ == "__main__":
    print("Running as main")
    TemplateFunction("MAIN")
