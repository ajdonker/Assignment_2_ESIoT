class LandingPattern: public Pattern{
  public:
    bool match(const Msg& m) override {
        return m.getContent() == "[DRU]:LAND";
    }
};