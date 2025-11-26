
class TakeOffPattern: public Pattern{
  public:
    bool match(const Msg& m) override {
        return m.getContent() == "[DRU]:TAKE_OFF";
    }
};