#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <vector>
struct Point
{
    int x, y;
    Point() : Point(0, 0) {}
    Point(float x, float y) : x(x), y(y) {}
    bool operator==(const Point& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Point& rhs) const
    {
        return !operator==(rhs);
    }
    Point operator+(const Point& rhs) const
    {
        return Point(x + rhs.x, y + rhs.y);
    }
    Point operator-(const Point& rhs) const
    {
        return Point(x - rhs.x, y - rhs.y);
    }
};
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
    };
int player;
static const int SIZE = 15;
std::vector<Point> next_valid_spots;
const int MAXDEPTH = 3;
std::array<std::array<int, SIZE>, SIZE> board;
class GomokuBoard {
public:
    int empty_count;
    int cur_player;
    bool done;
    int winner;
    std::array<std::array<int, SIZE>, SIZE> board;
    static const int SIZE = 15;
    std::vector<Point> next_valid_spots;
private:
    int get_next_player(int player) const {
        return 3 - player;
    }
    bool is_spot_on_board(Point p) const {
        return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
    }
    int get_disc(Point p) const {
        return board[p.x][p.y];
    }
    void set_disc(Point p, int disc) {
        board[p.x][p.y] = disc;
    }
    bool is_disc_at(Point p, int disc) const {
        if (!is_spot_on_board(p))
            return false;
        if (get_disc(p) != disc)
            return false;
        return true;
    }
    bool is_spot_valid(Point center) const {
        if (get_disc(center) != EMPTY)
            return false;
        return true;
    }
    
public:
    GomokuBoard() {
        reset();
    }
    void reset() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = EMPTY;
            }
        }
        cur_player = BLACK;
        empty_count = SIZE*SIZE;
        done = false;
        winner = -1;
    }
    bool put_disc(Point p) {
        if(!is_spot_valid(p)) {
            winner = get_next_player(cur_player);
            done = true;
            return false;
        }
        set_disc(p, cur_player);
        empty_count--;
        // Check Win
        if (checkwin(cur_player)) {
            done = true;
            winner = cur_player;
        }
        if (empty_count == 0) {
            done = true;
            winner = EMPTY;
        }

        // Give control to the other player.
        cur_player = get_next_player(cur_player);
        return true;
    }
    bool checkwin(int disc){
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (is_disc_at(Point(i, j), disc)){
                    bool iswin = true;
                    if (i + 4 < SIZE) {
                        for(int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i+k, j), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                    iswin = true;
                    if (j + 4 < SIZE) {
                        for(int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i, j+k), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                    iswin = true;
                    if (i + 4 < SIZE && j + 4 < SIZE) {
                        for(int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i+k, j+k), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                    iswin = true;
                    if (i - 4 >= 0 && j + 4 < SIZE) {
                        for(int k = 0; k < 5; k++)
                            if (!is_disc_at(Point(i-k, j+k), disc)) {
                                iswin = false;
                                break;
                            }
                        if (iswin) return true;
                    }
                }
            }
        }
        return false;
    }
    int value_board()
    {
        //cari value item

        //isi val setiap ada 3 item/4/5
        int val = 0;
        for(int x = 0; x < SIZE;x++)
        {
            for(int y = 0; y < SIZE ; y++)
            {
                
                if(board[x][y] == 1)
                {
                    // x=x
                    // y=y
                    //udah ketemu 1 item di x,y
                    //kanan
                    //cout<<board[x][y];
                    //1. cari lanjutan item di (x,y+1)(x,y+2) ...dst  v
                    //2. minimal loop 3 atau max loop 5 v
                    int it, count;
                    count = 1;
                    it = 1;
                    
                    if(board[x][y-it] != 1)
                    {
                        while(true)//buat y sampek brenti
                        {
                        if(board[x][y+it] != 1){//mau batesin sampe berenti
                            break;
                        }
                        count++;
                        it++;
                        } 
                    }
                
                    if(count >= 3 && count <= 5)
                    {
                        val++;
                    }
                    count = 1;
                    it = 1;
                    if(board[x-it][y] != 1)
                    {
                        while(true)//buat x sampek brenti
                        {
                            
                        if(board[x + it][y] != 1){//mau batesin sampe berenti
                            break;
                        }
                        count++;
                        it++;
                        
                        } 
                    }
                
                    if(count >= 3 && count <= 5)
                    {
                        val++;
                        std::cout<<"Countitem = "<<count<<std::endl;
                    }
                    count = 1;
                    it = 1;
                    if(board[x-it][y-it] != 1)
                    {
                        while(true)//buat y sampek brenti
                        {
                    
                        if(board[x + it][y+it] != 1){//mau batesin sampe berenti
                            break;
                        }
                        count++;
                        it++;
                        }
                    }
                
                    if(count >= 3 && count <= 5)
                    {
                        val++;
                        std::cout<<"Countitem = "<<count<<std::endl;
                    }
                    count = 1;
                    it = 1;
                    if(board[x-it][y+it] != 1)
                    {
                        while(true)//buat y sampek brenti
                        {
                        if(board[x + it][y-it] != 1){//mau batesin sampe berenti
                            break;
                        }
                        count++;
                        it++;
                        }
                    }
                
                    // dapet count buat ngitung tiap disc, jadi count adalah isi count = byk item yg didapat
                    if(count >= 3 && count <= 5)
                    {
                        val++;
                        std::cout<<"Countitem = "<<count<<std::endl;

                    }
                    count = 1;
                    it = 1;
                
                    // bawah (x+1,y),(x+2,y)
                    // diagonal kanan(x+1,y+1)(x+2,y+2)
                    // diagonal kiri(x+1,y-1)
                }//mau bikin kalau putih value--
                else if(board[x][y] == 2)
                {
                   int it, count;
                    count = 1;
                    it = 1;
                    while(true)//buat y sampek brenti
                    {
                    if(board[x][y+it] != 2){//mau batesin sampe berenti
                        break;
                    }
                    count++;
                    it++;
                    
                    } 
                    if(count >= 4 && count <= 5)
                    {
                        val--;
                        std::cout<<"Countputih = "<<count<<" ";
                        //std::cout<<"value = "<< val<<" ";
                    }
                    count = 1;
                    it = 1; 
                    while(true)//buat x sampek brenti
                    {
                    if(board[x + it][y] != 2){//mau batesin sampe berenti
                        break;
                    }
                    count++;
                    it++;
                    } 
                    if(count >= 4 && count <= 5)
                    {
                        val--;
                    }
                    count = 1;
                    it = 1;
                    
                    while(true)//buat y sampek brenti
                    {
                    
                    if(board[x + it][y+it] != 2){//mau batesin sampe berenti
                        break;
                    }
                    count++;
                    it++;
                    }
                    if(count >= 4 && count <= 5)
                    {
                        val--;
                    }
                    count = 1;
                    it = 1;
                    while(true)//buat y sampek brenti
                    {
                    if(board[x + it][y-it] != 2){//mau batesin sampe berenti
                        break;
                    }
                    count++;
                    it++;
                    }
                    // dapet count buat ngitung tiap disc, jadi count adalah isi count = byk item yg didapat
                    if(count >= 4 && count <= 5)
                    {
                        val--;
                    }
                    count = 1;
                    it = 1;
                }
            }
        }
        std::cout<<"val = "<< val<<" ";
    }
};

int index;
void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

int MiniMax(const GomokuBoard now, int depth, int A, int B, int player)
{
    int n_valid_spots = now.next_valid_spots.size();

    if (depth == 0 || n_valid_spots <= 0)
        return stateValue(now);

    if (player == 1)  // player's turn (maximize value)
    {
        int value = -1000;
        for (int i = 0; i < n_valid_spots; i++)
        {
            GomokuBoard next = now;
            next.put_disc(now.next_valid_spots[i]);
            int temp = MiniMax(next, depth - 1, A, B, 3 - player);
            if (temp > value)
            {
                value = temp;
                if(depth == MAXDEPTH)
                    index = i;
            }
            if (value > A)
            {
                A = value;
            }
            if (A >= B)
                break;
        }
        return value;
    }
    else if (player == 2)  // enemy's turn (minimize value)
    {
        int value = 1000;
        for (int i = 0; i < n_valid_spots; i++)
        {
            GomokuBoard next = now;
            next.put_disc(now.next_valid_spots[i]);
            int temp = MiniMax(next, depth - 1, A, B, 3 - player);
            if (temp < value)
            {
                value = temp;
                if(depth == MAXDEPTH)
                    index = i;
            }
            if (value < B)
            {
                B = value;
            }
            if (B <= A)
                break;
        }
        return value;
    }
}

void write_valid_spot(std::ofstream& fout) {
    srand(time(NULL));
    int x = 4 ;
    int y = 1;
    // Keep updating the output until getting killed.
    while(true) {
        // Choose a random spot.
        // int x = (rand() % SIZE);
        // int y = (rand() % SIZE);
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
            break;
        }
        else
        {
            x--;
            y++;
        }
    }
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);

    GomokuBoard g;
    
    read_board(fin);
    g.value_board();
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
