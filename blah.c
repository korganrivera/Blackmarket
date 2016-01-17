    //  time of day.
    if(time<12.0){
        if(last->firsttime){
            printf("it is morning. ");
            last->tod = 0;
            last->firsttime = 0;
        }
        else if(last->tod != 0){ printf("it is morning. "); last->tod = 0; }
    }

    else if(time<17.0){
        if(last->firsttime){
            printf("it is afternoon. ");
            last->tod = 1;
            last->firsttime = 0;
        }
        else if(last->tod != 1){ printf("it is afternoon. "); last->tod = 1; }
    }

    else if(time<20.0){
        if(last->firsttime){
            printf("it is evening. ");
            last->tod = 2;
            last->firsttime = 0;
        }
        else if(last->tod != 2){ printf("it is evening. "); last->tod = 2; }
    }

    else {
        if(last->firsttime){
            printf("it is night. ");
            last->tod = 3;
            last->firsttime = 0;
        }
        else if(last->tod != 3){ printf("it is afternoon. "); last->tod = 3; }
    }
