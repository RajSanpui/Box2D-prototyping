To use the debug draw feature in your main class -
  
    m_debugLayer = new DebugLayer(m_world);
    this->addChild(m_debugLayer, 9999);
 
 This should come right after world initialization.
