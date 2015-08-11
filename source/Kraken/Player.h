#ifndef PLAYER_H
#define PLAYER_H

#include "../Logic/Entity.h"
#include "../Logic/Camera.h"
#include "KrakenGameTiledMap.h"
#include "Predator.h"
#include "../Gosu/Core.h"
#include "Group.h"

// EXTRA - MUSIC PLAYER
enum SONG_NAMES           { TITLE, BONUS, EASY, NORMAL, HARD };
enum SFX_NAMES            { TO_BONUS, DESTROYED, PICKUP_50, PICKUP_100, BOOST, SLOWDOWN };

static void playSoundFX(SFX_NAMES sfx)
{
    switch(sfx)
    {
        case TO_BONUS:
            Core::get().getSoundfxResources().get("Bonus_Sweep")->play(0.3);
            break;
        case DESTROYED:
            Core::get().getSoundfxResources().get("Destroyed")->play(0.3);
            break;
        case PICKUP_50:
            Core::get().getSoundfxResources().get("Pickup50")->play(0.10);
            break;
        case PICKUP_100:
            Core::get().getSoundfxResources().get("Pickup100")->play(0.06);
            break;
        case BOOST:
            Core::get().getSoundfxResources().get("Boost")->play(0.17);
            break;
        case SLOWDOWN:
            Core::get().getSoundfxResources().get("Slowdown")->play(0.3);
            break;
    }
}

static void playSong(SONG_NAMES song)
{
    switch(song)
    {
        case TITLE:
            Core::get().getSongResources().get("Tune")->play(true);
            Core::get().getSongResources().get("Bonus")->stop();
            Core::get().getSongResources().get("Easy")->stop();
            Core::get().getSongResources().get("Normal")->stop();
            Core::get().getSongResources().get("Hard")->stop();
            break;
        case BONUS:
            Core::get().getSongResources().get("Bonus")->play(true);
            Core::get().getSongResources().get("Tune")->stop();
            Core::get().getSongResources().get("Easy")->stop();
            Core::get().getSongResources().get("Normal")->stop();
            Core::get().getSongResources().get("Hard")->stop();
            break;
        case EASY:
            Core::get().getSongResources().get("Easy")->play(true);
            Core::get().getSongResources().get("Bonus")->stop();
            Core::get().getSongResources().get("Tune")->stop();
            Core::get().getSongResources().get("Normal")->stop();
            Core::get().getSongResources().get("Hard")->stop();
            break;
        case NORMAL:
            Core::get().getSongResources().get("Normal")->play(true);
            Core::get().getSongResources().get("Bonus")->stop();
            Core::get().getSongResources().get("Easy")->stop();
            Core::get().getSongResources().get("Tune")->stop();
            Core::get().getSongResources().get("Hard")->stop();
            break;
        case HARD:
            Core::get().getSongResources().get("Hard")->play(true);
            Core::get().getSongResources().get("Bonus")->stop();
            Core::get().getSongResources().get("Easy")->stop();
            Core::get().getSongResources().get("Normal")->stop();
            Core::get().getSongResources().get("Tune")->stop();
            break;
    }
}

static void stopAllSongs()
{
    Core::get().getSongResources().get("Tune")->stop();
    Core::get().getSongResources().get("Bonus")->stop();
    Core::get().getSongResources().get("Easy")->stop();
    Core::get().getSongResources().get("Normal")->stop();
    Core::get().getSongResources().get("Hard")->stop();
}

const double PI_DIV_180 = (22.0/7.0)/180.0;

float RandomFloat(float a, float b);

class ParticleEmitter 
{
private:
    int max;
    double direction;
    int spread;
    Vector2D velCoord;
    Vector2D positionCoord;
    double minScale;
    double maxScale;
    int lifeMin; 
    int lifeMax;
    int alphaMin; 
    int alphaMax;
    int minR; 
    int maxR;
    int minG; 
    int maxG;
    int minB; 
    int maxB;
    int layer;
    
    Gosu::Image* img;
    
    class Particle
	{
	private:
		Vector2D positionCoord, velCoord;
		double scaleValue;
		int maxLifeValue, lifeValue;
		Gosu::Color colorValue;
        
	public:
		Particle(){positionCoord.x = positionCoord.y =
            velCoord.x = velCoord.y = scaleValue = 0.0;
            
            lifeValue = 0; colorValue = Gosu::Colors::white;}
        
		~Particle(){;}
		
        Vector2D &position()  { return positionCoord; }
        Vector2D &velocity()  { return velCoord;     }
		double &scale()       { return scaleValue;    }
        
		const bool removeThis() const{if(lifeValue<=0) return true; return false;}
		
        Gosu::Color color() {return Gosu::Color(
                                                ((double)lifeValue/(double)maxLifeValue)*255,
                                                colorValue.red(), 
                                                colorValue.green(), 
                                                colorValue.blue());}
		
		void setLife(const int l)
		{
			maxLifeValue = l;
			lifeValue = l;
        }
        
        void setColor(Gosu::Color c)
        {
            colorValue = c;
        }
        
		void update()
		{
			lifeValue -= 1;
			positionCoord.x += velCoord.x;
            positionCoord.y += velCoord.y;
            
            velCoord.x *= 0.98;
            velCoord.y *= 0.98;
		}
        
	};
    
    Group<Particle> particle_group;
    
public:
    
	ParticleEmitter()
	{
		max = 100;
		direction = 90;
		spread = 10;
		velCoord.x = 1.0;
        velCoord.y = 1.0;
		minScale = 1.0; maxScale = 1.0;
		lifeMin = 0; lifeMax = 100;
		alphaMin = 254; alphaMax = 255;
		minR = 0; maxR = 255;
		minG = 0; maxG = 255;
		minB = 0; maxB = 255;
        
        img = 0;
        
        layer = 0;
	}
	
	~ParticleEmitter()
	{
	    particle_group.clear();	
	}
	
    void setDirection(double angle)
    {
        this->direction = angle;    
    }
    
    void setSpread(int spr)
    {
        spread = spr;
    }
    
    void setLayer(int l)
    {
        layer = l;
    }
    
	void setImage(Gosu::Image* image)
	{
		img = image;
	}
    
    Vector2D &position() { return positionCoord; }
	Vector2D &velocity() { return velCoord; }
    
	void add()
	{
        if(particle_group.getSize() > max)
            return;
            
		double variation = Gosu::random(-spread/2.0, spread/2.0);
		double dir = direction;
		double vx = cosf((dir + variation) * PI_DIV_180); 
		double vy = sinf((dir + variation) * PI_DIV_180);
        
		int r = ((maxR-minR) > 0)? rand()%(maxR-minR)+minR : minR; 
		int g = ((maxG-minG) > 0)? rand()%(maxG-minG)+minG : minG; 
		int b = ((maxB-minB) > 0)? rand()%(maxB-minB)+minB : minB; 
		int a = ((alphaMax-alphaMin) > 0)? rand()%(alphaMax-alphaMin)+alphaMin : alphaMin; 
		int l = ((lifeMax-lifeMin) > 0)? rand()%(lifeMax-lifeMin)+lifeMin : lifeMin; 		
        double scale = RandomFloat(minScale, maxScale); 
		
		Particle p;
		p.position() = Vector2D(position().x, position().y);
		p.velocity() = Vector2D(vx*velCoord.x, vy*velCoord.y);
        
        p.setLife(l);
		p.setColor(Gosu::Color(0, 0, 255, a));
		p.scale() = scale;
		
		particle_group.add(p);
	}
    
	void draw(Camera *cam) 
	{ 
		for(int i = 0; i < particle_group.getSize() ; i++) 
		{ 
            Vector2D drawable = cam->worldToScreen(particle_group.get(i).position());
            
			img->draw(
					  drawable.x,
					  drawable.y, 
					  layer, 
					  cam->getZoom()*particle_group.get(i).scale(), 
					  cam->getZoom()*particle_group.get(i).scale(),
                      Gosu::Color(particle_group.get(i).color().alpha(), 255, 255, 255),
                      Gosu::amAdditive
                      );
		} 
	} 
	
	void draw()
	{
		for(int i = 0; i < particle_group.getSize() ; i++) 
		{ 
			img->draw(
                      particle_group.get(i).position().x,
                      particle_group.get(i).position().y, 
                      layer, 
                      particle_group.get(i).scale(), 
                      particle_group.get(i).scale(), 
                      particle_group.get(i).color()
                      );
		} 	
	}
	
	void update()
	{
		for(int i = 0; i < particle_group.getSize() ; i++) 
		{ 
			particle_group.get(i).update(); 
			//is this particle's life over? 
			if(particle_group.get(i).removeThis()) 
			{ 
				//reset particle
				particle_group.remove(i);
                i--;
                continue;
			} 
		} 
	} 
    
    void setScaleRange(double min, double max)
    {
        minScale = min;
        maxScale = max;
    }
	
	void setLifeRange(int min, int max)
	{
		lifeMin = min;
		lifeMax = max;
	}
	
	void setAlphaRange(int min, int max) 
	{ 
		alphaMin = min; 
		alphaMax = max; 
	} 
	
	void setColorRange(int r1, int g1, int b1, int r2, int g2, int b2) 
	{ 
		minR = r1; maxR = r2; 
		minG = g1; maxG = g2; 
		minB = b1; maxB = b2; 
	} 	
    
    void setMax(int m)
    {
        max = m;
    }
};

class SpeedBlur : public Entity
{
private:
    Vector2D pos;
    Gosu::Image* img;
    double alpha;
    double alpha_speed;
    
    Camera *cam;
    
public:
    SpeedBlur(Gosu::Image *image, Camera* camera, Vector2D position, double start_alpha, double alpha_speed) :
    Entity()
    {
        pos = position;
        img = image;
        cam = camera;
        
        if(start_alpha > 255)
            start_alpha = 255;
        
        alpha = start_alpha;
        
        this->alpha_speed = alpha_speed;
    }
    
    void onUpdate()
    {
        alpha -= alpha_speed;
        
        if(alpha <= 0.0)
            removeEntity();
    }
    
    void onDraw()
    {
        double   scale = cam->getZoom();
        Vector2D coord = cam->worldToScreen(Vector2D(pos.x-8, pos.y-8));
        
        Gosu::Color c = Gosu::Colors::white;
        c.setAlpha(alpha);
        img->draw(coord.x, coord.y, 4, scale, scale, c, Gosu::amAdditive);
    }
};

class Player : public Entity
{
protected:
    KrakenGameTiledMap *map;
    Camera *camera;
    
    Vector2D position; // on screen
    int row; 
    
    double speed;
    
    bool did_boost;
    
    bool game_over;
    
    bool complete_map; 
    
    int collected_money;
    
    bool in_transition;
    
    ParticleEmitter smoke_emitter, boost_emitter, money_emitter;
    
public:
    Player(int, int);
    ~Player();
    
    virtual void onUpdate() = 0;
    virtual void onDraw() = 0;
    
    const int getX();
    const int getY();
    
    const double getSpeed();
    
    void noticeMap(KrakenGameTiledMap *map);
    void noticeCamera(Camera* cam);
    
    void emitBoostParticles();
    void emitMoneyParticles();
    
    const bool isPointInside(Vector2D point);
    
    const bool isGameOver() { return game_over; }
    
    const bool hasCompleteMap() { return complete_map; }
    
    void signalGameOver() { game_over = true; }
    
    void pollInput();
    
    const int getMoney() { return collected_money; }
};

class CarPlayer : public Player
{    
private:
    int FLY_COUNT; // There's a total of three
    
public:
    CarPlayer(int, int);
    ~CarPlayer();
    
    void onUpdate();
    void onDraw();
    
    const bool gotoPlaneMinigame() { return (FLY_COUNT == 7); }
    const int  getBalloonCount()   { return FLY_COUNT;        }
    void  unflagPlaneMinigame()    { FLY_COUNT = 0;           }
};

class PlanePlayer : public Player
{   
public:
    PlanePlayer(int, int);
    ~PlanePlayer();
    
    void onUpdate();
    void onDraw();
};

class PlayerGUI : public Entity
{
private:
    Player* watchPlayer;
    KrakenGameTiledMap* map;
    Predator          * pred;
    
    Gosu::Font *font;
    int cash_counter;
    int total_cash; // by 50s
    int cash_needed; // by 50s
    
    int base_cash; // For whenever there's a lock
    
    bool lock_collected_as_min;
    
public:
    
    PlayerGUI(int money_needed_this_level) : Entity()
    {
        watchPlayer  = 0;
        map          = 0;
        pred         = 0;
        total_cash   = 0;
        cash_counter = 0;
        base_cash    = 0;
        
        font = new Gosu::Font(Core::get().getGraphics(), L"Helvetica", 20);
        font->setImage(L'$', *Core::get().getGraphicResources().get("Money"));
        
        cash_needed = money_needed_this_level;
        
        lock_collected_as_min = false;
    }
    
    ~PlayerGUI() { ; }
    
    void noticePlayer(Player* p) { watchPlayer = p; }
    void noticeMap(KrakenGameTiledMap *m) { map = m; }
    void noticePredator(Predator *p) { pred = p; }
    void setLockAmount(int money) { lock_collected_as_min = true; base_cash = money; }
    void changeCashTo(int money)  { base_cash = money - total_cash;                  }
    
    const int getTotalCash() { return total_cash; }
    const int getCashNeeded() { return cash_needed; }
    
    void onUpdate() 
    { 
        total_cash = base_cash + watchPlayer->getMoney();
        
        if(cash_counter < total_cash*5)
        {
            cash_counter++;
            
        }
        
        if(cash_counter > total_cash*5)
        {
            cash_counter-=5;
        }
        
        if(cash_counter < total_cash*5 && lock_collected_as_min)
        {
            cash_counter = total_cash*5;
        }
        
        if(cash_counter < 0)
            cash_counter = 0;
    }
    
    std::string addCommasToLargeNumber(long number)
    {
        std::string result;
        
        bool isbig = false;
        
        while((double)(number/1000) > 0)
        {
            isbig = true;
            
            long front = number/1000;
            
            long back = number - (front*1000);
            
            result += boost::lexical_cast<std::string>(front);
            result += ",";
            
            number = back;
        }
        
        if (number == 0) 
        {
            if(isbig)
                result += "000";
            else
                result += "0";
        }
        else if (number == 500)
        {
            result += "00";
        }
        else
        {
            result += boost::lexical_cast<std::string>(number);
        } 
         
        return result;
    }
    
    void writeNote(const std::string note)
    {
        font->drawRel(Gosu::widen(note), Core::get().getScreenWidth()/2.0, 50, 12, 0.5, 0.5, 2, 2, 
                      Gosu::Colors::black);
        
        font->drawRel(Gosu::widen(note), Core::get().getScreenWidth()/2.0, 48, 12, 0.5, 0.5, 2, 2, 
                      Gosu::Colors::white);
    }
    
    void onDraw()   
    { 
        // Draw money and money goal        
        std::string text = "$";
        text += addCommasToLargeNumber(cash_counter*10);
        text += " out of $";
        text += addCommasToLargeNumber(cash_needed*50);
        
        font->drawRel(Gosu::widen(text), Core::get().getScreenWidth()/2.0, Core::get().getScreenHeight() - 18, 12, 0.5, 0.5, 2, 2, 
                      Gosu::Colors::black);
        
        if(cash_counter <= total_cash*5)
            font->drawRel(Gosu::widen(text), Core::get().getScreenWidth()/2.0, Core::get().getScreenHeight() - 20, 12, 0.5, 0.5, 2, 2);
        else
            font->drawRel(Gosu::widen(text), Core::get().getScreenWidth()/2.0, Core::get().getScreenHeight() - 20, 12, 0.5, 0.5, 2, 2,
                          Gosu::Colors::red);
        
        // Draw line and two dots. Red being player; Blue being other.
        // This represents place on map and what's left to go.
        Core::get().getGraphics().drawQuad( 100, 15, Gosu::Colors::white,
                                           Core::get().getScreenWidth() - 100, 15, Gosu::Colors::white,
                                           Core::get().getScreenWidth() - 100, 25, Gosu::Colors::gray,
                                           100, 25, Gosu::Colors::gray, 12 );
        
        // Dot red: Player
        Vector2D tri_point_1 = Vector2D(0, -10 + 20);
        Vector2D tri_point_2 = Vector2D(10, 0  + 20);
        Vector2D tri_point_3 = Vector2D(0, 10  + 20);
        
        // Find length of this line: 100px off both ends
        // length = ScreenWidth - 200
        int length = Core::get().getScreenWidth() - 200;
        
        // The line is the relation of the map. Find the scale.
        // scale = RealMapLength / length
        double scale = (double)length / (double)(map->getMapWidth()*map->getCellWidth());
        
        // Now scale and place the position
        // point_x = (object.x * scale) + 100px
        
        tri_point_1.x += (watchPlayer->getX() * scale) + 100;
        tri_point_2.x += (watchPlayer->getX() * scale) + 100;
        tri_point_3.x += (watchPlayer->getX() * scale) + 100;
        
        Core::get().getGraphics().drawTriangle(tri_point_1.x, tri_point_1.y, Gosu::Colors::red,
                                               tri_point_2.x, tri_point_2.y, Gosu::Colors::red,
                                               tri_point_3.x, tri_point_3.y, Gosu::Colors::red, 12);
        
        // Predator may not be defined for this game mode! check!!!
        
        if(pred != 0)
        {
            // Draw blue: opponent
            tri_point_1 = Vector2D(0, -10 + 20);
            tri_point_2 = Vector2D(10, 0  + 20);
            tri_point_3 = Vector2D(0, 10  + 20);
            
            tri_point_1.x += (pred->getPosition().x * scale) + 100;
            tri_point_2.x += (pred->getPosition().x * scale) + 100;
            tri_point_3.x += (pred->getPosition().x * scale) + 100;
            
            Core::get().getGraphics().drawTriangle(tri_point_1.x, tri_point_1.y, Gosu::Colors::blue,
                                                   tri_point_2.x, tri_point_2.y, Gosu::Colors::blue,
                                                   tri_point_3.x, tri_point_3.y, Gosu::Colors::blue, 12);
        }
    }
};

#endif