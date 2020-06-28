Declaration is same -

  
  However, the difference is how the Sprite is moved - 
  
    void HelloWorld::ccTouchesMoved(Touch* touch, Event* evento)
    {
      CCLOG("************** Touch event called ***************");
      auto currPosition = touch->getLocation();
      if(ball1->getBoundingBox().containsPoint(currPosition)){
          CCLOG("************** Grounding event called ***************");
          b2Vec2 vel(currPosition.x, currPosition.y);
          dynamicBody2->SetLinearVelocity( vel );
      }
    }
    
    void HelloWorld::initTouch()
   {   
     auto listener = EventListenerTouchOneByOne::create();
     listener -> onTouchBegan = [] (Touch* touch, Event* event) { return true;};
     listener -> onTouchMoved = CC_CALLBACK_2(HelloWorld::ccTouchesMoved, this);
     listener -> onTouchEnded = [=] (Touch* touch, Event* event) {};   
     _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener, this);
    }
