#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
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
class GomokuBoard
{
public:
    enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
    };
int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;
int empty_count;
int cur_player;
bool done;
int winner;
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
void put_disc(Point p)
{
    board[p.x][p.y] = player;
    player = 3 - player;
}
bool is_valid(Point p) const
{
    return(board[p.x][p.y] == 0);//x verti y hori
}
int value_board()
{
    //cari value item

    //isi val setiap ada 3 item/4/5
    int val = 0;
    for(int i = 0; i < SIZE;i++)
    {
        for(int j = 0; j < SIZE ; j++)
        {
            if(board[p.x][p.y] == 1)
            {
                    // x=p.x
                    // y=p.y
                //udah ketemu 1 item di x,y
                //kanan
                //cout<<board[p.x][p.y];
                //1. cari lanjutan item di (x,y+1)(x,y+2) ...dst  v
                //2. minimal loop 3 atau max loop 5 v
                int count = 0;
                while(true)//buat y sampek brenti
                {
                    if(board[p.x][p.y+count] != 1 ||board[p.x + count][p.y] != 1||board[p.x + count][p.y + count] != 1||board[p.x + count][p.y - count] != 1){//mau batesin sampe berenti
                        break;
                    }
                    count++;
                }
                // dapet count buat ngitung tiap disc, jadi count adalah isi count = byk item yg didapat
                if(count >= 3 && count <= 5)
                {
                    val++;
                }
                j += count;
                // bawah (x+1,y),(x+2,y)
                // diagonal kanan(x+1,y+1)(x+2,y+2)
                // diagonal kiri(x+1,y-1)
            }//mau bikin kalau putih value--
            else if(board[p.x][p.y] == 2)
            {
                while(true){
                    if(board[p.x][p.y+count] != 2 ||board[p.x + count][p.y] != 2||board[p.x + count][p.y + count] != 2||board[p.x + count][p.y - count] != 2){//mau batesin sampe berenti
                        break;
                    }
                    count++;
                }
                if(count >= 4 && count <= 5)
                {
                    val--;
                }
                j += count;
            }
            
        }
    }
}


};


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
            OthelloBoard next = now;
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
    int x, y;
    // Keep updating the output until getting killed.
    while(true) {
        // Choose a random spot.
        int x = (rand() % SIZE);
        int y = (rand() % SIZE);
        if (board[x][y] == EMPTY) {
            fout << x << " " << y << std::endl;
            // Remember to flush the output to ensure the last action is written to file.
            fout.flush();
        }
    }
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
