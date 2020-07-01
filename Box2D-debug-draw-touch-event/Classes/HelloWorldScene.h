

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GLES-Render.h"
#include "MyContactListener.h"
#include "DebugLayer.h"

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

    cocos2d::Mat4 _modelViewMV;

    b2Body* groundBody;
    b2Body* dynamicBody;
    cocos2d::Sprite *boulder;
    b2FixtureDef boulderfixture;
    b2BodyDef boulderdef;

    MyContactListener *contactListener;
    void touch(cocos2d::Point location);

    // implement the "static create()" method manually
    void initTouch();
    void ccTouchesMoved(cocos2d::Touch* touch, cocos2d::Event* evento);

    DebugLayer *m_debugLayer;

    CREATE_FUNC(HelloWorld);

    //GLESDebugDraw m_debugDraw;
};

#endif // __HELLOWORLD_SCENE_H__
