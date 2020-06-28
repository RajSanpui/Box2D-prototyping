

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GLES-Render.h"
#include "MyContactListener.h"

enum class PhysicsCategory {
    None = 0,
    Bomba = (1 << 0),    // 1
    Box = (1 << 1), // 2
    All = PhysicsCategory::Bomba | PhysicsCategory::Box // 3
};

class HelloWorld : public cocos2d::Scene
//class HelloWorld : public cocos2d::CCLayer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    b2World* m_world;

    void tick(float dt);
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    //virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
    cocos2d::Mat4 _modelViewMV;
    //void onDraw();
    cocos2d::CustomCommand _customCommand;
    //GLESDebugDraw m_debugDraw;
    b2Body* groundBody;
    // implement the "static create()" method manually

    MyContactListener *contactListener;

    CREATE_FUNC(HelloWorld);

    //GLESDebugDraw m_debugDraw;
};

#endif // __HELLOWORLD_SCENE_H__
