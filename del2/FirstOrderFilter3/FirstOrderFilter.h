#ifndef FIRSTORDERFILTER_H
#define FIRSTORDERFILTER_H



class FirstOrderFilter
{
public:
  FirstOrderFilter(double breakFrequency );
  ~FirstOrderFilter();

  double lavPass(double input, double deltaTime);

private:
  double breakFrequency_;
  double timeConstant_;
  
  double weightFactor1_;
  double weightFactor2_;
  double output_;
  double previousOutput_;



  
  
};

#endif // FIRSTORDERFILTER_H


