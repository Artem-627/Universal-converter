import os


os.system("./Converter")

def convert_to(number, base, upper=True):
    sign = 0
    if number < 0:
        sign = 1
        number = -number
    digits = '0123456789abcdefghijklmnopqrstuvwxyz'
    if base > len(digits): return None
    result = ''
    while number > 0:
        result = digits[number % base] + result
        number //= base
    if sign:
        result = '-' + result
    return result.upper() if upper else result

base = 2

for a in range(-10, 10):
    for b in range(-10, 10):
        a_conv = convert_to(a, base)
        b_conv = convert_to(b, base)

        print(a_conv, b_conv)

        res = input()
        if (convert_to(a + b, base) == res):
            print("OK")
        else:
            print("ERROR")
