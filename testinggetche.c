/*
    Testing to see if I can read arrow keys without entering newline, and
    anything else with a newline.
    start:  2015.9.17.20:49
    end:    2015.9.17.21:07     Seems to work.  I'm sure there's a downside of using getch but I can't think what it is.
*/

#include <stdio.h>

int main(void){

    char c;

    do{
        c = getch();

        if(c==-32){
            c=getch();
            printf("\n%c", c);
            if(c=='H') printf("\nyou entered ^");
            else if(c=='P') printf("\nyou entered v");
            else if(c=='K') printf("\nyou entered <");
            else if(c=='M') printf("\nyou entered >");
            else printf("\nyou entered escape + %c (%d)", c, c);
        }
        else printf("\nyou entered %c", c);

    }while(c!='x');


}
