a <- input();;
b <- input();;
c <- (a * b) % 10;;
if (c == 0 || a > b):
    status <- 2;;
    log(status, a, b);;
else:
    status <- 3;;
    if (b % 2 == 1):
        correction <- b + 1;;
    else:
        correction <- b;;
    adjust(correction);;
output(status)