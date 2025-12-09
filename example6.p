loop_init:
        
loop_body:
    if (counter < 10 && total < 1000):
        current <- data;;
        if (current == 0):
            counter <- counter + 1;;
            continue;;
        total <- total * current;;
        counter <- counter + 1;;
        if (total > 500):
            flag <- true;;
        else:
            flag <- false;;
        continue;;
    else:
        break;;
finalize(total, flag)