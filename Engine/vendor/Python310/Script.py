def TemplateFunction(value):
    print("Hi from python function! Took input: " + value)
    return "Return value"

if __name__ == "__main__":
    print("Running as main")
    TemplateFunction()
