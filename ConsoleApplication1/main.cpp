#include <iostream>
#include "Game.hpp"


/*Nên làm thêm :
    T?i ?u code 
      chia ra thành snake food map object ui gamelogic
    
    Làm game restart ?c t?i menu và gameover;
     
    L?y high score 
      score ?c hi?n khi ?ang ch?i 

    
    Map    + borderless: ?ã có
           + border    : Chuy?n block c?a các ô ngoài thành wall
           + Maze      : t?o 1 maze ??n gi?n v?i các block wall
    Mode   + Speed : ?n ???c food thì có th? (ng?u nhiên) t?ng t?c ho?c gi?m t?c       (trong th?i gian gi?i h?n ho?c border map)
           + MysteriousFood : Food có th? là - ??c1 - tr? Body
                                               ??c2 - ??o h??ng di chuy?n ( ng?u nhiên - ??i ng??c )
                                               ?? ?n - + 1
            .....
    Áp d?ng ?? h?a;
      R?n::??u r?n - ??u r?n ?ang ?n - thân r?n - r?n ??i h??ng - ?uôi r?n và ?? ?n:: ?? ?n( tùy mode)   == "snake1.png"
      Map:: map
        

    T?i ?u l?n 2
*/


int main(int argc, char* argv[])
{
    Game game;
    game.Run();
    
    return 0;
}