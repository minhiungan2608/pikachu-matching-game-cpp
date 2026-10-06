#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <queue>
#include <random>
#include <stdexcept>
#include <tuple>
#include <vector>
#include "game.h"
#include "create_window.h"

struct GameTestAccess {
    static int (&board(Game& g))[GAME_ROW][GAME_COLUMN] { return g.board; }
    static Game& game(Create& c) { return *c.game; }
    static void expire(Game& g) { g.in_game_time = 420; }
    static void exhaust(Game& g) { g.count_change = -1; }
};
int checks = 0;
void expect(bool ok, const char* description) {
    ++checks;
    if(!ok) throw std::runtime_error(description);
}
bool reference_path(int b[][GAME_COLUMN], int ar, int ac, int br, int bc) {
    if(ar<1 || ar>=GAME_ROW-1 || br<1 || br>=GAME_ROW-1 ||
       ac<1 || ac>=GAME_COLUMN-1 || bc<1 || bc>=GAME_COLUMN-1 ||
       (ar==br && ac==bc) || b[ar][ac]==0 || b[ar][ac]!=b[br][bc]) return false;
    const int dr[]={-1,0,1,0}, dc[]={0,1,0,-1};
    int best[GAME_ROW][GAME_COLUMN][4];
    for(auto& row:best) for(auto& cell:row) for(auto& value:cell) value=99;
    std::queue<std::tuple<int,int,int,int>> q;
    for(int d=0;d<4;++d) { q.emplace(ar,ac,d,0);best[ar][ac][d]=0; }
    while(!q.empty()) {
        int r,c,d,t;std::tie(r,c,d,t)=q.front();q.pop();
        for(int nd=0;nd<4;++nd) {
            int nr=r+dr[nd],nc=c+dc[nd],nt=t+(d!=nd);
            if(nr<0 || nr>=GAME_ROW || nc<0 || nc>=GAME_COLUMN || nt>2) continue;
            if(nr==br && nc==bc) return true;
            if(b[nr][nc]!=0 || best[nr][nc][nd]<=nt) continue;
            best[nr][nc][nd]=nt;q.emplace(nr,nc,nd,nt);
        }
    }
    return false;
}
void path_tests() {
    Game game;
    auto& b=GameTestAccess::board(game);
    b[2][2]=b[2][3]=1;
    expect(game.find_way(2,2,2,3),"Adjacent identical tiles should match");
    expect(!game.find_way(2,2,2,2),"Same tile must not match itself");
    expect(!game.find_way(0,0,2,2),"Border endpoints must be rejected");
    expect(!game.find_way(2,2,3,3),"Empty endpoints must be rejected");
    b[2][3]=2;
    expect(!game.find_way(2,2,2,3),"Different tile IDs must not match");
    std::mt19937 rng(2608);
    for(int trial=0;trial<1000;++trial) {
        for(int r=0;r<GAME_ROW;++r) for(int c=0;c<GAME_COLUMN;++c)
            b[r][c]=(r && c && r<GAME_ROW-1 && c<GAME_COLUMN-1 && rng()%3) ? 2 : 0;
        int ar=1+rng()%9,ac=1+rng()%16,br=1+rng()%9,bc=1+rng()%16;
        if(ar==br && ac==bc) bc=bc%16+1;
        b[ar][ac]=b[br][bc]=1;
        expect(game.find_way(ar,ac,br,bc)==reference_path(b,ar,ac,br,bc),
            "Path validation differs from independent two-turn graph search");
    }
    std::memset(b,0,sizeof(b));
    b[1][1]=b[1][16]=3; b[1][8]=4;
    expect(game.find_way(1,1,1,16),"Empty outer border must support a two-turn route");
    expect(game.check_possible(),"Available matching pair must be found");
    int histogram[5]={};for(auto& row:b)for(int v:row)++histogram[v];
    game.change();
    int shuffled[5]={};for(auto& row:b)for(int v:row)++shuffled[v];
    expect(std::equal(histogram,histogram+5,shuffled),"Shuffle must preserve tile counts");
    std::memset(b,0,sizeof(b));
    expect(game.check_complete(),"Empty board must be complete");
    expect(!game.check_possible(),"Empty board must have no playable pair");
}
void deletion_tests() {
    using Delete=void(*)(int[][GAME_COLUMN],int,int,int,int);
    const Delete remove[]={Game_Delete::lv1_delete,Game_Delete::lv2_delete,
        Game_Delete::lv3_delete,Game_Delete::lv4_delete,Game_Delete::lv5_delete,
        Game_Delete::lv6_delete,Game_Delete::lv7_delete};
    std::mt19937 rng(42);
    for(int level=1;level<=7;++level) for(int trial=0;trial<150;++trial) {
        int actual[GAME_ROW][GAME_COLUMN]={}, expected[GAME_ROW][GAME_COLUMN]={};
        for(int r=1;r<GAME_ROW-1;++r)for(int c=1;c<GAME_COLUMN-1;++c)
            actual[r][c]=expected[r][c]=r*100+c;
        int ar=1+rng()%9,ac=1+rng()%16,br=1+rng()%9,bc=1+rng()%16;
        if(ar==br && ac==bc) bc=bc%16+1;
        if(level==1) { expected[ar][ac]=expected[br][bc]=0; }
        else {
            int segments=(level<=3)?16:((level>=6)?18:9);
            for(int s=0;s<segments;++s) {
                std::vector<std::pair<int,int>> coordinates;
                bool pad_front=false;
                if(level<=3) {
                    for(int r=1;r<=9;++r) coordinates.emplace_back(r,s+1);
                    pad_front=(level==2);
                } else {
                    int row=(level>=6)?s/2+1:s+1;
                    int start=(level>=6 && s%2)?9:1;
                    int end=(level>=6 && !(s%2))?8:16;
                    for(int c=start;c<=end;++c) coordinates.emplace_back(row,c);
                    pad_front=(level==4 || (level==6 && s%2) || (level==7 && !(s%2)));
                }
                std::vector<int> values;
                for(auto p:coordinates)
                    if(p!=std::make_pair(ar,ac) && p!=std::make_pair(br,bc))
                        values.push_back(expected[p.first][p.second]);
                int zeros=static_cast<int>(coordinates.size()-values.size());
                if(pad_front) values.insert(values.begin(),zeros,0);
                else values.insert(values.end(),zeros,0);
                for(size_t i=0;i<coordinates.size();++i)
                    expected[coordinates[i].first][coordinates[i].second]=values[i];
            }
        }
        remove[level-1](actual,ar,ac,br,bc);
        expect(std::memcmp(actual,expected,sizeof(actual))==0,"Level deletion violates reference shift rule");
    }
}
void push_click(int x,int y,Create& app,SDL_Renderer*& renderer) {
    SDL_FlushEvents(SDL_FIRSTEVENT,SDL_LASTEVENT);
    SDL_Event e{};e.type=SDL_MOUSEBUTTONDOWN;e.button.button=SDL_BUTTON_LEFT;e.button.x=x;e.button.y=y;
    expect(SDL_PushEvent(&e)==1,"Mouse event could not be queued");
    app.handle(renderer); app.render(renderer);
}
void runtime_tests() {
    SDL_Window* window=nullptr;SDL_Renderer* renderer=nullptr;Create app;
    try {
        app.init("QA",0,0,SCREEN_WIDTH,SCREEN_HEIGHT,false,window,renderer);
        app.play_soundtrack();app.render(renderer);
        expect(app.get_game_state()==0,"Game must begin at menu");
        push_click(600,500,app,renderer);
        expect(app.get_game_state()==1,"Play must enter level one");
        if(const char* path=SDL_getenv("PIKACHU_CAPTURE_SCREENSHOT")) {
            SDL_Surface* image=SDL_CreateRGBSurfaceWithFormat(0,SCREEN_WIDTH,SCREEN_HEIGHT,32,SDL_PIXELFORMAT_RGBA32);
            expect(image!=nullptr,"Screenshot surface could not be created");
            expect(SDL_RenderReadPixels(renderer,nullptr,image->format->format,image->pixels,image->pitch)==0,
                "Could not read rendered frame");
            expect(IMG_SavePNG(image,path)==0,"Could not save rendered screenshot");
            SDL_FreeSurface(image);
        }
        Game& game=GameTestAccess::game(app);
        auto& b=GameTestAccess::board(game);
        int tile_count[25]={};
        for(auto& row:b)for(int v:row)++tile_count[v];
        expect(tile_count[0]==54,"Board must have an empty border");
        for(int n=1;n<=24;++n)expect(tile_count[n]==6,"Each tile ID must occur six times");
        int before[GAME_ROW][GAME_COLUMN];std::memcpy(before,b,sizeof(b));
        SDL_Event offboard{};offboard.type=SDL_MOUSEBUTTONDOWN;offboard.button.button=SDL_BUTTON_LEFT;
        offboard.button.x=183;offboard.button.y=140;game.handle(&offboard,renderer);
        expect(std::memcmp(before,b,sizeof(b))==0,"Off-board click must not change tiles");
        std::memset(b,0,sizeof(b));
        b[1][1]=b[1][2]=1; b[2][1]=b[2][2]=2;
        push_click(210,166,app,renderer);
        expect(b[1][1]==1,"First click must select without removing a tile");
        push_click(262,166,app,renderer);
        expect(b[1][1]==0 && b[1][2]==0 && b[2][1]==2,
            "Two matching clicks must remove only the selected pair");
        push_click(1100,150,app,renderer);
        expect(app.get_game_state()==9,"Pause button must enter pause state");
        push_click(600,500,app,renderer);
        expect(app.get_game_state()==1,"Continue must resume the level");
        std::memset(b,0,sizeof(b));
        push_click(200,160,app,renderer);
        expect(app.get_game_state()==10,"Cleared board must enter level-complete state");
        push_click(600,500,app,renderer);
        expect(app.get_game_state()==2,"Continue must enter next level");
        GameTestAccess::expire(game);
        SDL_FlushEvents(SDL_FIRSTEVENT,SDL_LASTEVENT);app.handle(renderer);app.render(renderer);
        expect(app.get_game_state()==11,"Expired timer must enter loss state");
        push_click(600,500,app,renderer);
        expect(app.get_game_state()==1,"Play again must restart level one");
        GameTestAccess::exhaust(game);
        push_click(200,160,app,renderer);
        expect(app.get_game_state()==11,"Exhausted shuffle allowance must enter loss state");
        push_click(600,650,app,renderer);
        expect(app.get_game_state()==0,"Loss menu must return to main menu");
        for(int level=1;level<=7;++level) {
            game.init(level,renderer,level-1);game.render(renderer);
            game.render_pause(renderer);game.render_win(renderer);game.render_lose(renderer);
            expect(game.get_count()==level,"Next level must add one shuffle allowance");
        }
        Timer timer;timer.start();SDL_Delay(30);timer.pause();int paused=timer.getTicks();
        SDL_Delay(30);expect(timer.getTicks()==paused,"Paused timer must not advance");
        timer.unpause();SDL_Delay(30);expect(timer.getTicks()>=paused+20,"Resumed timer must advance");
        timer.stop();expect(timer.getTicks()==0,"Stopped timer must reset");
        SDL_FlushEvents(SDL_FIRSTEVENT,SDL_LASTEVENT);
        SDL_Event quit{};quit.type=SDL_QUIT;SDL_PushEvent(&quit);app.handle(renderer);
        expect(app.get_game_state()==12,"Quit event must end game");
        app.clean(window,renderer);
    } catch(...) { app.clean(window,renderer);throw; }
}
int main() {
    SDL_SetMainReady();
    try {
        path_tests();deletion_tests();runtime_tests();
        std::cout<<"PASS: "<<checks<<" checks (path search, seven shift rules, SDL runtime, timer).\n";
        return 0;
    } catch(const std::exception& error) { std::cerr<<"FAIL: "<<error.what()<<'\n';return 1; }
}
