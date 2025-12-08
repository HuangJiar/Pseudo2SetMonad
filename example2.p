loop_init:
    b <- 1;;
    a <- x
loop_body:
    if (n > 0):
        if (n % 2 == 1):
            b <- b * a;;
        a <- a * a;;
        n <- n / 2;;
        continue
    else:
        break