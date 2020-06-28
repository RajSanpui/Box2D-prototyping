

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GLES-Render.h"
#include "DebugLayer.h"
#include "MyContactListener.h"
#include "../cocos2d/external/Box2D/include/Box2D/Dynamics/b2Body.h"
#include "../cocos2d/external/Box2D/include/Box2D/Dynamics/b2Fixture.h"
#include "../cocos2d/external/bullet/include/bullet/BulletDynamics/Character/btKinematicCharacterController.h"

#define PTM_RATIO 32.0
#define FLOOR_HEIGHT 62.0f

USING_NS_CC;


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object

    return HelloWorld::create();
    //return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label


    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    m_world = new b2World(gravity);
    m_world->SetContinuousPhysics(true);


    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    groundBody = m_world->CreateBody(&groundBodyDef);

    CCPoint myPosition = getPosition();
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

    float widthInMeters = screenSize.width*1.0f / PTM_RATIO;
    float maxHeightInMeters = screenSize.height / PTM_RATIO;
    float minHeightInMeters = FLOOR_HEIGHT / PTM_RATIO;
    b2Vec2 lowerLeftCorner = b2Vec2(0, minHeightInMeters);
    b2Vec2 lowerRightCorner = b2Vec2(widthInMeters, minHeightInMeters);
    b2Vec2 upperLeftCorner = b2Vec2(0, maxHeightInMeters);
    b2Vec2 upperRightCorner = b2Vec2(widthInMeters, maxHeightInMeters);

    // Define the static container body, which will provide the collisions at screen borders.
    b2BodyDef screenBorderDef;
    screenBorderDef.position.Set(0, 0);
    b2Body* screenBorderBody = m_world->CreateBody(&screenBorderDef);
    b2EdgeShape screenBorderShape;

    // Create fixtures for the four borders (the border shape is re-used)
    screenBorderShape.Set(lowerLeftCorner, lowerRightCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);
    screenBorderShape.Set(lowerRightCorner, upperRightCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);
    screenBorderShape.Set(upperRightCorner, upperLeftCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);
    screenBorderShape.Set(upperLeftCorner, lowerLeftCorner);
    screenBorderBody->CreateFixture(&screenBorderShape, 0);

    ball = Sprite::create("bomba.png");
    //ball->setPosition(100, 200);
    this->addChild(ball);

    ball1 = Sprite::create("bomba.png");
    //ball1->setPosition(100, 150);
    this->addChild(ball1);

    initTouch();

    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0); //position, relative to body position
    circleShape.m_radius = 26.0/PTM_RATIO;
    //circleShape.m_radius = 1; //radius

    b2BodyDef myBodyDef1;
    myBodyDef1.type = b2_dynamicBody; //this will be a dynamic body
    myBodyDef1.position.Set(100/PTM_RATIO, 200/PTM_RATIO); //a little to the left
    myBodyDef1.userData = ball;

    //debugLayer->setPosition(30,20);
    dynamicBody1 = m_world->CreateBody(&myBodyDef1);

    b2FixtureDef myFixtureDef;
    myFixtureDef.shape = &circleShape; //this is a pointer to the shape above
    myFixtureDef.density = 1.0f;
    myFixtureDef.friction = 0.2f;
    myFixtureDef.restitution = 1.0f;
    myFixtureDef.filter.categoryBits = (uint16)PhysicsCategory::Bomba;
    myFixtureDef.filter.maskBits = (uint16)PhysicsCategory::Bomba;
    dynamicBody1->CreateFixture(&myFixtureDef); //add a fixture to the body


    // Ball2 initialization
    b2BodyDef myBodyDef2;
    myBodyDef2.type = b2_dynamicBody; //this will be a dynamic body
    myBodyDef2.position.Set(100/PTM_RATIO, 150/PTM_RATIO); //a little to the left
    myBodyDef2.userData = ball1;

    //debugLayer->setPosition(30,20);
    dynamicBody2 = m_world->CreateBody(&myBodyDef2);

    b2FixtureDef myFixtureDef1;
    myFixtureDef1.shape = &circleShape; //this is a pointer to the shape above
    myFixtureDef1.density = 1.0f;
    myFixtureDef1.friction = 0.2f;
    myFixtureDef1.restitution = 1.0f;
    dynamicBody2->CreateFixture(&myFixtureDef1); //add a fixture to the body
    myFixtureDef1.filter.categoryBits = (uint16)PhysicsCategory::Bomba;
    myFixtureDef1.filter.maskBits = (uint16)PhysicsCategory::Bomba;

    // Initialize contact listener for collision detection
    contactListener = new MyContactListener();
    m_world->SetContactListener(contactListener);

    // Applying linear velocity
    b2Vec2 vel(10, 0); // Only towards x-direction
    dynamicBody2->SetLinearVelocity( vel );

    // Applying force
    dynamicBody1->ApplyForce( b2Vec2(10,0), dynamicBody1->GetWorldCenter(), true );

    // Applying impulse
    float velChange = -3.0f;
    float impulse = dynamicBody1->GetMass() * velChange; //disregard time factor
    dynamicBody1->ApplyLinearImpulse( b2Vec2(impulse,0), dynamicBody1->GetWorldCenter(), true );

    //Box2D v2.2.1 onwards - Now body1 has no effect on gravity
    dynamicBody1->SetGravityScale(0);//cancel gravity (use -1 to reverse gravity, etc)

    // Rotating at a given angle
    b2Vec2 toTarget(-140, 180);
    float desiredAngle = atan2f( -toTarget.x, toTarget.y );
    dynamicBody1->SetTransform( dynamicBody1->GetPosition(), desiredAngle );

    // Applying torque
    float32 totalRotation = 10.0f;
    dynamicBody1->ApplyTorque( totalRotation, true );

    schedule( schedule_selector(HelloWorld::tick) );

    return true;
}

void HelloWorld::initTouch()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener -> onTouchBegan = [] (Touch* touch, Event* event) { return true;};
    listener -> onTouchMoved = CC_CALLBACK_2(HelloWorld::ccTouchesMoved, this);
    listener -> onTouchEnded = [=] (Touch* touch, Event* event) {};
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener, this);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::ccTouchesMoved(Touch* touch, Event* evento)
{
    CCLOG("************** Touch event called ***************");
    auto currPosition = touch->getLocation();
    if(ball1->getBoundingBox().containsPoint(currPosition)){
        CCLOG("************** Grounding event called ***************");
        b2Vec2 vel(currPosition.x, currPosition.y);
        //b2Vec2 vel(currPosition.x, 0);
        dynamicBody2->SetLinearVelocity( vel );
    }
}

void HelloWorld::tick(float dt)
{
    m_world->Step(dt, 10, 10);
    for(b2Body *b = m_world->GetBodyList(); b; b=b->GetNext()) {
        if (b->GetUserData() != NULL) {
            CCSprite *ballData = (CCSprite *)b->GetUserData();
            ballData->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            ballData->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }
    }
}
