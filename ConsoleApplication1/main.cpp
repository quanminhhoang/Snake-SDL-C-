#include <iostream>
#include "Game.hpp"


/*N�n l�m th�m :
    T?i ?u code 
      chia ra th�nh snake food map object ui gamelogic
    
    L�m game restart ?c t?i menu v� gameover;
     
    L?y high score 
      score ?c hi?n khi ?ang ch?i 

    
    Map    + borderless: ?� c�
           + border    : Chuy?n block c?a c�c � ngo�i th�nh wall
           + Maze      : t?o 1 maze ??n gi?n v?i c�c block wall
    Mode   + Speed : ?n ???c food th� c� th? (ng?u nhi�n) t?ng t?c ho?c gi?m t?c       (trong th?i gian gi?i h?n ho?c border map)
           + MysteriousFood : Food c� th? l� - ??c1 - tr? Body
                                               ??c2 - ??o h??ng di chuy?n ( ng?u nhi�n - ??i ng??c )
                                               ?? ?n - + 1
            .....
    �p d?ng ?? h?a;
      R?n::??u r?n - ??u r?n ?ang ?n - th�n r?n - r?n ??i h??ng - ?u�i r?n v� ?? ?n:: ?? ?n( t�y mode)   == "snake1.png"
      Map:: map
        

    T?i ?u l?n 2
*/


int main(int argc, char* argv[])
{
    Game game;
    game.Run();
    
    return 0;
}