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
    // friend ostream& operator<<(ostream& os, const Point& p)
    // {
    //     os << p.x << '/' << p.y ;
    //     return os;
    // }
};


enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
    };
static const int SIZE = 15;
const int MAXDEPTH = 4;
int player_game;

class GomokuBoard {
public:
    int empty_count;
    int cur_player;
    bool done;
    int winner;
    std::array<std::array<int, SIZE>, SIZE> board;
    std::vector<Point> next_valid_spots;
    GomokuBoard& operator=(const GomokuBoard& rhs)
    {
        this->empty_count = rhs.empty_count;
        this->cur_player = rhs.cur_player;
        this->done = rhs.done;
        this->winner = rhs.winner;
        this->board = rhs.board;
        this->next_valid_spots = rhs.next_valid_spots;
        return *this;
    }

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
    GomokuBoard(const GomokuBoard& rhs) {
        this->empty_count = rhs.empty_count;
        this->cur_player = rhs.cur_player;
        this->done = rhs.done;
        this->winner = rhs.winner;
        this->board = rhs.board;
        this->next_valid_spots = rhs.next_valid_spots;
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
        next_valid_spots = get_valid_spots();
    }
    std::vector<Point> get_valid_spots()
    {
        std::vector<Point> valid_spots;
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                Point p = Point(i, j);
                if (board[i][j] != EMPTY)
                    continue;
                if (is_spot_valid(p))
                {
                    if((((i)>=0 && (i)<SIZE && (j + 1)>=0 && SIZE>(j + 1)) && board[i][j+1] != 0)
                    ||(((i + 1)>=0 && (i + 1)<SIZE && (j)>=0 && SIZE>(j)) && board[i+1][j] != 0)
                    ||(((i + 1)>=0 && (i + 1)<SIZE && (j - 1)>=0 && SIZE>(j - 1)) && board[i+1][j-1] != 0)
                    ||(((i)>=0 && (i)<SIZE && (j - 1)>=0 && SIZE>(j - 1)) && board[i][j-1] != 0)
                    ||(((i - 1)>=0 && (i - 1)<SIZE && (j)>=0 && SIZE>(j)) && board[i-1][j] != 0)
                    ||(((i - 1)>=0 && (i - 1)<SIZE && (j - 1)>=0 && SIZE>(j - 1)) && board[i-1][j-1] != 0)
                    ||(((i - 1)>=0 && (i - 1)<SIZE && (j + 1)>=0 && SIZE>(j + 1)) && board[i-1][j+1] != 0)
                    ||(((i + 1)>=0 && (i + 1)<SIZE && (j + 1)>=0 && SIZE>(j + 1)) && board[i+1][j+1] != 0))
                    {
                        valid_spots.push_back(p);
                    }  
                }
            }
        }
        if(valid_spots.size() == 0){
            //std::cout<<"KOSONG\n";
            valid_spots.push_back(Point(8, 8));
        }else{
            //std::cout<<"GAKOSONG\n";
            for (int i = 0; i < valid_spots.size(); i++) {
                Point p = valid_spots[i];
                //std::cout << p.x<<','<< p.y<< " ";
            }
        }
        next_valid_spots = valid_spots;
        return valid_spots;
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
        next_valid_spots = get_valid_spots();
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
    int value_board() const
    {
        //cari value item

        //isi val setiap ada 3 item/4/5
        int val = 0;
        for(int x = 0; x < SIZE;x++)
        {
            for(int y = 0; y < SIZE ; y++)
            {
                
                if(board[x][y] == player_game)
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
                    
                    if(board[x][y-it] != player_game)
                    {
                        while(true)//buat y sampek brenti
                        {
                        if(board[x][y+it] != player_game){//mau batesin sampe berenti
                            break;
                        }
                        count++;
                        it++;
                        } 
                    }
                
                    if(count == 3){
                    val++;
                    }else if(count == 4){
                    val+=10;
                    }else if (count == 5){
                    val+=100;
                    }
                    count = 1;
                    it = 1;
                    if(board[x-it][y] != player_game)
                    {
                        while(true)//buat x sampek brenti
                        {
                            
                        if(board[x + it][y] != player_game){//mau batesin sampe berenti
                            break;
                        }
                        count++;
                        it++;
                        
                        } 
                    }
                
                    if(count == 3){
                    val++;
                    }else if(count == 4){
                    val+=10;
                    }else if (count == 5){
                    val+=100;
                    }
                    count = 1;
                    it = 1;
                    if(board[x-it][y-it] != player_game)
                    {
                        while(true)//buat y sampek brenti
                        {
                    
                        if(board[x + it][y+it] != player_game){//mau batesin sampe berenti
                            break;
                        }
                        count++;
                        it++;
                        }
                    }
                
                    if(count == 3){
                    val++;
                    }else if(count == 4){
                    val+=10;
                    }else if (count == 5){
                    val+=100;
                    }
                    count = 1;
                    it = 1;
                    if(board[x-it][y+it] != player_game)
                    {
                        while(true)//buat y sampek brenti
                        {
                        if(board[x + it][y-it] != player_game){//mau batesin sampe berenti
                            break;
                        }
                        count++;
                        it++;
                        }
                    }
                
                    // dapet count buat ngitung tiap disc, jadi count adalah isi count = byk item yg didapat
                    if(count == 3){
                    val++;
                    }else if(count == 4){
                    val+=10;
                    }else if (count == 5){
                    val+=100;
                    }
                    count = 1;
                    it = 1;
                
                    // bawah (x+1,y),(x+2,y)
                    // diagonal kanan(x+1,y+1)(x+2,y+2)
                    // diagonal kiri(x+1,y-1)
                }//mau bikin kalau putih value--
                else if(board[x][y] == 3 - player_game)
                {
                    int it, count;
                    count = 1;
                    it = 1;
                    while(true)//buat y sampek brenti
                    {
                    if(board[x][y+it] != 3 - player_game){//mau batesin sampe berenti
                        break;
                    }
                    count++;
                    it++;
                    
                    } 
                    if(count == 3){
                    val--;
                    }else if(count == 4){
                    val-=10;
                    }else if (count == 5){
                    val-=100;
                    }
                    count = 1;
                    it = 1; 
                    while(true)//buat x sampek brenti
                    {
                    if(board[x + it][y] != 3 - player_game){//mau batesin sampe berenti
                        break;
                    }
                    count++;
                    it++;
                    } 
                    if(count == 3){
                    val--;
                    }else if(count == 4){
                    val-=10;
                    }else if (count == 5){
                    val-=100;
                    }
                    count = 1;
                    it = 1;
                    
                    while(true)//buat y sampek brenti
                    {
                    
                    if(board[x + it][y+it] != 3 - player_game){//mau batesin sampe berenti
                        break;
                    }
                    count++;
                    it++;
                    }
                    if(count == 3){
                    val--;
                    }else if(count == 4){
                    val-=10;
                    }else if (count == 5){
                    val-=100;
                    }
                    count = 1;
                    it = 1;
                    while(true)//buat y sampek brenti
                    {
                    if(board[x + it][y-it] != 3 - player_game){//mau batesin sampe berenti
                        break;
                    }
                    count++;
                    it++;
                    }
                    // dapet count buat ngitung tiap disc, jadi count adalah isi count = byk item yg didapat
                    if(count == 3){
                    val--;
                    }else if(count == 4){
                    val-=10;
                    }else if (count == 5){
                    val-=100;
                    }
                    count = 1;
                    it = 1;
                }
            }
        }
        return val;
    }
};

GomokuBoard cur_board;

void read_board(std::ifstream& fin) {
    fin >> player_game;
    cur_board.cur_player = player_game;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> cur_board.board[i][j];
        }
    }
}
int idx;
int MiniMax(const GomokuBoard now, int depth, int A, int B, int player)
{
    int n_valid_spots = now.next_valid_spots.size();

    if (depth == 0 || n_valid_spots <= 0){
        return now.value_board();        
        /*for(int i=0;i<SIZE;i++){
            for(int j=0;j<SIZE;j++){
                std::cout << now.board[i][j]<<" ";
            }
            std::cout<<"\n";
        }*/
    }

    if (player == player_game)  // player's turn (maximize value)
    {
        int value = -1000;
        for (int i = 0; i < n_valid_spots; i++)
        {
            GomokuBoard next(now);
            next.put_disc(now.next_valid_spots[i]);
            int temp = MiniMax(next, depth - 1, A, B, 3 - player);
            if (temp > value)
            {
                value = temp;
                if(depth == MAXDEPTH)
                    idx = i;
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
    else if (player == 3 - player_game)  // enemy's turn (minimize value)
    {
        int value = 1000;
        for (int i = 0; i < n_valid_spots; i++)
        {
            GomokuBoard next(now);
            next.put_disc(now.next_valid_spots[i]);
            int temp = MiniMax(next, depth - 1, A, B, 3 - player);
            if (temp < value)
            {
                value = temp;
                if(depth == MAXDEPTH)
                    idx = i;
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
    cur_board.get_valid_spots();
    Point p;
    p = cur_board.next_valid_spots[0];
    int temp = MiniMax(cur_board, MAXDEPTH, -1000, 1000, cur_board.cur_player);
    p = cur_board.next_valid_spots[idx]; 
    fout << p.x << " " << p.y << std::endl;
    fout.flush();
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
