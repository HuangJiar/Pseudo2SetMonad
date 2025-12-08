loop_init:
    sum <- 0;;
    i <- 0;;
loop_body:
    if (i < 5):
        j <- 0;;
        if (j < 3):
            sum <- sum + multiply(i, j);;
            j <- j + 1;;
            continue;;
        else:
            break;;
        i <- i + 1;;
        continue;;
    else:
        break;;
report(sum)