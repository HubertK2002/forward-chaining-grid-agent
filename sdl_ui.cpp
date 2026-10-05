#include "sdl_ui.h"

#include "agent_logic.h"
#include "history.h"
#include "state.h"

#include <SDL.h>
#include <SDL_ttf.h>

#include <algorithm>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
struct SdlContext {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    SdlContext(const char* title, int width, int height) {
        SDL_SetMainReady();
        if (SDL_Init(SDL_INIT_VIDEO) != 0) throw std::runtime_error(SDL_GetError());
        if (TTF_Init() != 0) throw std::runtime_error(TTF_GetError());
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  width, height, SDL_WINDOW_RESIZABLE);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!window || !renderer) throw std::runtime_error(SDL_GetError());
        const char* paths[] = {
            "C:/Windows/Fonts/consola.ttf", "C:/Windows/Fonts/arial.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/truetype/liberation2/LiberationSans-Regular.ttf"
        };
        for (const char* path : paths) if (std::filesystem::exists(path)) {
            font = TTF_OpenFont(path, 16);
            if (font) break;
        }
        if (!font) throw std::runtime_error("No usable TTF font found (DejaVu Sans, Liberation Sans, or Arial)");
    }
    ~SdlContext() {
        if (font) TTF_CloseFont(font);
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        TTF_Quit(); SDL_Quit();
    }
};
void color(SDL_Renderer* r, SDL_Color c) { SDL_SetRenderDrawColor(r, c.r,c.g,c.b,c.a); }
void text(SdlContext& c, const std::string& s, int x, int y, SDL_Color fg={230,236,245,255}) {
    SDL_Surface* surf = TTF_RenderUTF8_Blended(c.font, s.c_str(), fg);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(c.renderer, surf);
    SDL_Rect dst{x,y,surf->w,surf->h};
    SDL_RenderCopy(c.renderer,tex,nullptr,&dst);
    SDL_DestroyTexture(tex); SDL_FreeSurface(surf);
}
void button(SdlContext& c, SDL_Rect rect, const char* label, bool active=false) {
    color(c.renderer, active ? SDL_Color{39,106,131,255} : SDL_Color{42,52,67,255});
    SDL_RenderFillRect(c.renderer,&rect);
    color(c.renderer,{78,94,115,255}); SDL_RenderDrawRect(c.renderer,&rect);
    text(c,label,rect.x+12,rect.y+10);
}
void present(SDL_Renderer* r) { SDL_RenderPresent(r); }
void begin(SDL_Renderer* r) { color(r,{17,22,30,255}); SDL_RenderClear(r); }

void drawMap(SdlContext& c) {
    SDL_Renderer* r=c.renderer;
    int windowW=960, windowH=720;
    const int panelW=320, pad=28;
    SDL_GetWindowSize(c.window,&windowW,&windowH);
    int cell=std::max(16,std::min(54,(windowW-panelW-2*pad)/std::max(1,Width)));
    int mapH=Height*cell;
    int mapX=pad, mapY=std::max(pad,(windowH-mapH)/2);
    for(int y=0;y<Height;y++) for(int x=0;x<Width;x++) {
        SDL_Rect cellRect{mapX+x*cell,mapY+y*cell,cell-2,cell-2};
        SDL_Color fill{34,43,55,255};
        if(Obstacles[y][x]) fill=ObstaclesObserved[y][x] ? SDL_Color{201,116,65,255} : SDL_Color{74,81,91,255};
        if(DoorPosition.x==x&&DoorPosition.y==y) fill={54,157,115,255};
        if(IsAgentAtPosition(x,y)) fill={70,154,220,255};
        color(r,fill); SDL_RenderFillRect(r,&cellRect);
        color(r,{53,64,79,255}); SDL_RenderDrawRect(r,&cellRect);
        if(IsAgentAtPosition(x,y)) text(c,"A",cellRect.x+cell/2-5,cellRect.y+cell/2-10);
        else if(DoorPosition.x==x&&DoorPosition.y==y) text(c,"D",cellRect.x+cell/2-5,cellRect.y+cell/2-10);
        else if(Obstacles[y][x]) text(c,ObstaclesObserved[y][x]?"O":"?",cellRect.x+cell/2-5,cellRect.y+cell/2-10);
    }
    int panelX=std::max(mapX+Width*cell+24,windowW-panelW);
    text(c,"PROJECT 1 | AGENT",panelX,28,{115,211,196,255});
    text(c,"A agent   D door   O seen obstacle   ? hidden",panelX,60);
    text(c,"ENTER run/pause   SPACE step   M maps   ESC quit",panelX,88);
    text(c,"Recent observations and inferences",panelX,128,{170,184,204,255});
    const int start=std::max(0,(int)History.size()-18);
    int y=158;
    for(int i=start;i<(int)History.size();++i) {
        std::string line="["+History[i].kind+"] ";
        if(!History[i].premises.empty()) line+=History[i].premises.front()+" -> ";
        line+=History[i].conclusion;
        if(line.size()>42) line=line.substr(0,39)+"...";
        text(c,line,panelX,y,{196,205,218,255}); y+=25;
    }
    button(c,{panelX,650,132,42},"STEP (SPACE)");
    button(c,{panelX+145,650,132,42},"RUN (ENTER)");
    button(c,{panelX,704,277,38},"BACK TO MAPS (M)");
}
}

MapSelection ChooseMapSourceSDL(const std::string& directory) {
    const auto files=GetMapFiles(directory);
    SdlContext c("Project 1 — map selection",760,580);
    int selected=0, mode=0;
    RandomMapParameters p{5,12,{0,2},{11,2},30};
    int field=0;
    bool done=false,quit=false;
    while(!done&&!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type==SDL_QUIT) quit=true;
            if(e.type==SDL_KEYDOWN) {
                auto k=e.key.keysym.sym;
                if(mode==0) {
                    if(k==SDLK_DOWN) selected=std::min(selected+1,(int)files.size());
                    if(k==SDLK_UP) selected=std::max(0,selected-1);
                    if(k==SDLK_g) {mode=1;}
                    if(k==SDLK_RETURN&&selected==(int)files.size()) {mode=1;}
                    if(k==SDLK_RETURN&&selected<(int)files.size()) {done=true;}
                } else {
                    if(k==SDLK_TAB) field=(field+1)%5;
                    if(k==SDLK_UP||k==SDLK_DOWN) {
                        int d=k==SDLK_UP?1:-1;
                        if(field==0){p.height=std::clamp(p.height+d,3,7);p.agentPosition.y=std::min(p.agentPosition.y,p.height-1);p.doorPosition.y=std::min(p.doorPosition.y,p.height-1);}
                        if(field==1){p.width=std::clamp(p.width+d,7,20);p.doorPosition.x=p.width-1;}
                        if(field==2)p.agentPosition.y=std::clamp(p.agentPosition.y+d,0,p.height-1);
                        if(field==3)p.doorPosition.y=std::clamp(p.doorPosition.y+d,0,p.height-1);
                        if(field==4)p.obstacleProbabilityPercent=std::clamp(p.obstacleProbabilityPercent+d*10,0,90);
                    }
                    if(k==SDLK_RETURN)done=true;
                    if(k==SDLK_ESCAPE)mode=0;
                }
                if(k==SDLK_ESCAPE&&mode==0)quit=true;
            }
            if(e.type==SDL_MOUSEBUTTONDOWN) {
                int y=e.button.y;
                if(mode==0&&e.button.x<670&&y>=130&&y<130+(int)files.size()*42) {
                    selected=(y-130)/42;
                    if(e.button.clicks>1)done=true;
                }
                if(mode==0&&y>130+(int)files.size()*42) mode=1;
                if(mode==1&&y>390&&y<470) {field=std::clamp((e.button.x-40)/130,0,4);}
                if(mode==1&&y>490)done=true;
            }
        }
        begin(c.renderer);
        text(c,mode==0?"Choose a map":"Random map settings",36,30,{115,211,196,255});
        if(mode==0) {
            text(c,"Select a map and press ENTER, or press G for a generated map.",36,70);
            for(int i=0;i<(int)files.size();i++) button(c,{36,120+i*42,650,36},std::filesystem::path(files[i]).filename().string().c_str(),i==selected);
            button(c,{36,140+(int)files.size()*42,300,40},"G   RANDOM MAP",selected==(int)files.size());
        } else {
            const std::string names[]={"HEIGHT ","WIDTH ","AGENT ROW ","DOOR ROW ","OBSTACLES % "};
            const int vals[]={p.height,p.width,p.agentPosition.y,p.doorPosition.y,p.obstacleProbabilityPercent};
            text(c,"UP/DOWN adjusts selected value; TAB changes value.",36,80);
            for(int i=0;i<5;i++) {
                std::string label=names[i]+std::to_string(vals[i]);
                button(c,{36+i*137,180,128,58},label.c_str(),field==i);
            }
            button(c,{36,300,280,46},"GENERATE AND START");
        }
        present(c.renderer); SDL_Delay(16);
    }
    if(quit) throw std::runtime_error("Map selection cancelled");
    if(mode==1) { p.doorPosition.x=p.width-1; return {true,"",p}; }
    if(files.empty()) throw std::runtime_error("No map files found in directory: "+directory);
    return {false,files.at(std::clamp(selected,0,(int)files.size()-1)),{}};
}

bool RunAgentSDL() {
    SdlContext c("Project 1 — agent simulation",1280,760);
    bool quit=false,run=false,finished=false,returnToMaps=false;
    Uint32 last=SDL_GetTicks();
    while(!quit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type==SDL_QUIT) quit=true;
            if(e.type==SDL_KEYDOWN) {
                if(e.key.keysym.sym==SDLK_ESCAPE)quit=true;
                if(e.key.keysym.sym==SDLK_m)returnToMaps=true;
                if(e.key.keysym.sym==SDLK_RETURN)run=!run;
                if(e.key.keysym.sym==SDLK_SPACE&&!finished) {MoveAgent();MakeObservations();finished=IsAgentAtDoor();}
            }
            if(e.type==SDL_MOUSEBUTTONDOWN&&e.button.y>690&&e.button.x>=960&&e.button.x<1240) returnToMaps=true;
            if(e.type==SDL_MOUSEBUTTONDOWN&&e.button.y>640&&e.button.y<692&&!finished) {
                if(e.button.x>960&&e.button.x<1100) {MoveAgent();MakeObservations();finished=IsAgentAtDoor();}
                if(e.button.x>=1105&&e.button.x<1240) run=!run;
            }
        }
        if(returnToMaps) return true;
        Uint32 now=SDL_GetTicks();
        if(run&&!finished&&now-last>=500) {MoveAgent();MakeObservations();finished=IsAgentAtDoor();last=now;}
        int w,h; SDL_GetWindowSize(c.window,&w,&h);
        begin(c.renderer);
        drawMap(c);
        if(finished) text(c,"MISSION COMPLETE",w-300,h-72,{106,218,156,255});
        present(c.renderer); SDL_Delay(8);
    }
    return false;
}
