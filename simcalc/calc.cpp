
#include "calc.hpp"

using namespace std;


//math
void addvec(vec & vf, vec v1, vec v2);//add vectors v1+v2
void subvec(vec & vf, vec v1, vec v2);//subtract vectors v1-v2
float dotprod(vec v1, vec v2);//dot rpoduct
void normalize(vec & vf, vec v1);//normalize vector



//globals
float dragCd = 0;
float Xarea = 0;
float stmass = 0;
float wind = 3.5;

float tstep = 0.01;//seconds
float thrust = 0;//newtons
float btime = 0;//seconds
float promass = 0;//kg
float tmass = 0;//kg
float para = 0;
float vintx, vinty;
bool par = true;
bool thcurv = false;

float pp0 = 0.035;//rise time
float pp1 = 100;
float pp2 = 10;//height of lowlevel platue
float pp3 = 0.075;//center
float pp4 = 20;//height
float pp5 =0.075;//width
float pp6 = 0.25;//center


//main functions


void initialize(prock & rp, frock & rf, string filename)
{
const char* search1 = "% Wind:";
const char* search2 = "% Btime:";
const char* search3 = "% TMass:";
const char* search4 = "% DragCd:";
const char* search5 = "% Thrust:";
const char* search6 = "% TcxArea:";
const char* search7 = "% ProMass:";
const char* search8 = "% Vintx:";
const char* search9 = "% Vinty:";
const char* search10 = "% ParArea:";


const char* c = filename.c_str();

ifstream file(c);
		string str, temp, tomp; 
		string::size_type sz;
		while (getline(file, str))
		 { if (str.find(search1, 0) != string::npos)
				 {  getline(file, str);
				   wind = ::atof(str.c_str());   
					cout << "\nfound wind: " << wind;
		   		 }
		   		 
		   if (str.find(search2, 0) != string::npos)
				 {  getline(file, str);
				   btime = ::atof(str.c_str());   
					cout << "\nfound btime: " << btime;
		   		 }
		   		 
		   if (str.find(search3, 0) != string::npos)
				 {  getline(file, str);
				  tmass = ::atof(str.c_str());   
					cout << "\nfound tmass: " << tmass;
		   		 }
		   if (str.find(search4, 0) != string::npos)
				 {  getline(file, str);
				  dragCd = ::atof(str.c_str());   
					cout << "\nfound dragCd: " << dragCd;
		   		 }
		   		 
		   if (str.find(search5, 0) != string::npos)
				 {  getline(file, str);
				  thrust = ::atof(str.c_str());   
					cout << "\nfound thrust: " << thrust;
		   		 }
		   		 
		   if (str.find(search6, 0) != string::npos)
				 {  getline(file, str);
				  Xarea = ::atof(str.c_str());   
					cout << "\nfound area: " << Xarea;
		   		 }
		  if (str.find(search7, 0) != string::npos)
				 {  getline(file, str);
				  promass = ::atof(str.c_str());   
					cout << "\nfound promass: " << promass;
		   		 }
		   		 
		   if (str.find(search8, 0) != string::npos)
				 {  getline(file, str);
				  vintx = ::atof(str.c_str());   
					cout << "\nfound vx: " << vintx;
		   		 }
		   		 
		   if (str.find(search9, 0) != string::npos)
				 {  getline(file, str);
				  vinty = ::atof(str.c_str());   
					cout << "\nfound vy: " << vinty;
		   		 }
		   if (str.find(search10, 0) != string::npos)
				 {  getline(file, str);
				  para = ::atof(str.c_str());   
					cout << "\nfound vy: " << para;
		   		 }

		 }

		file.close();


rp.d.x = 0;
rp.d.y = 0;
rp.Ft.x = 0;
rp.Ft.y = 0;
rp.v.x = vintx;
rp.v.y = vinty;
rp.m = tmass;

rf.fd.x = 0;
rf.fd.y = 0;
rf.fw.x = 0;
rf.fw.y = 0;
rf.fg.x = 0;
rf.fg.y = 0;
rf.ft.x = 0;
rf.ft.y = 0;
stmass = promass/(btime/tstep);
}


void use_thrustCurve(string filename)
{


const char* search1 = "% Pp0:";
const char* search2 = "% Pp1:";
const char* search3 = "% Pp2:";
const char* search4 = "% Pp3:";
const char* search5 = "% Pp4:";
const char* search6 = "% Pp5:";
const char* search7 = "% Pp6:";



const char* c = filename.c_str();

ifstream file(c);
		string str, temp, tomp; 
		string::size_type sz;
		while (getline(file, str))
		 { if (str.find(search1, 0) != string::npos)
				 {  getline(file, str);
				   pp0 = ::atof(str.c_str());   
					//cout << "\nfound wind: " << pp0;
		   		 }
		   		 
		   if (str.find(search2, 0) != string::npos)
				 {  getline(file, str);
				   pp1 = ::atof(str.c_str());   
					//cout << "\nfound btime: " << pp1;
		   		 }
		   		 
		   if (str.find(search3, 0) != string::npos)
				 {  getline(file, str);
				  pp2 = ::atof(str.c_str());   
					//cout << "\nfound tmass: " << pp2;
		   		 }
		   if (str.find(search4, 0) != string::npos)
				 {  getline(file, str);
				  pp3 = ::atof(str.c_str());   
					//cout << "\nfound dragCd: " << pp3;
		   		 }
		   		 
		   if (str.find(search5, 0) != string::npos)
				 {  getline(file, str);
				  pp4 = ::atof(str.c_str());   
					//cout << "\nfound thrust: " << pp4;
		   		 }
		   		 
		   if (str.find(search6, 0) != string::npos)
				 {  getline(file, str);
				  pp5 = ::atof(str.c_str());   
					//cout << "\nfound area: " << pp5;
				 }
		   if (str.find(search7, 0) != string::npos)
				 {  getline(file, str);
				  pp6 = ::atof(str.c_str());   
					//cout << "\nfound area: " << pp6;
				 }
		}
file.close();

thcurv = true;

}
//--------------Calc & Sum forces---------------//

void calc_forces(prock & rp, frock & rf, float tstep)
{
rp.m = rp.m - stmass;
//drag calc
vec temp;
normalize(temp, rp.v);

rf.fd.y = -temp.y*(0.5)*(1.225)*pow(mag(rp.v),2)*(dragCd)*Xarea;

rf.fd.x = -temp.x*(0.5)*(1.225)*pow(mag(rp.v),2)*(dragCd)*Xarea;


//printf("Fdragx %f Fdragy %f\n", rf.fd.x , rf.fd.y);

//wind
rf.fw.x = -(0.5)*(1.225)*pow(wind,2)*(dragCd)*Xarea*2;

//printf("Fwindx %f Fwindy %f\n", rf.fw.x , rf.fw.y);


//gravity
rf.fg.y = rp.m*(-9.81);
rf.fg.x = 0;
//printf("fgravityx %f fgravityy %f\n", rf.fg.x , rf.fg.y);


//thrust
if(thcurv)
{
float x = tstep;
thrust = (pp4*exp(-0.5*pow((x-pp6)/pp5,2)))+(((x-pp3)>0)*(-(pp2)*(exp(-(x-pp3)/pp0)-exp(-(x-pp3)/pp1))));
 if(thrust < 0)
 thrust = thrust*(-1.0);
//printf("thrust %f \n", thrust);
}
normalize(temp, rp.v);
rf.ft.x = thrust*(temp.x);
rf.ft.y = thrust*(temp.y);
//printf("thrustx %f thrusty %f\n", rf.ft.x , rf.ft.y);
//sum fores
vec temp2;
addvec(temp, rf.ft, rf.fg);
addvec(temp2, temp, rf.fd);
addvec(rp.Ft, temp2, rf.fw);
//printf("Ftotalx %f Ftotaly %f\n\n", rp.Ft.x , rp.Ft.y);


}

//------------------Kinematics-----------------//

void calc_kinematics(prock & rp, float tstep)
{
vec vfinal;
vec dfinal;

vec a;

a.x = rp.Ft.x/rp.m;
a.y = rp.Ft.y/rp.m;

vfinal.x = rp.v.x + a.x*(tstep);
vfinal.y = rp.v.y + a.y*(tstep);


dfinal.x = rp.v.x*(tstep) + (0.5)*a.x*pow(tstep,2);
dfinal.y = rp.v.y*(tstep) + (0.5)*a.y*pow(tstep,2);

//printf("Intial vx %f, vy %f, dx %f, dy %f, m %f\n", rp.v.x, rp.v.y, rp.d.x, rp.d.y, rp.m);
if(rp.d.y >= 0)
{
rp.d.x = rp.d.x + dfinal.x;
rp.d.y = rp.d.y + dfinal.y;
rp.v.x = vfinal.x;
rp.v.y = vfinal.y;
}
else
{
rp.v.y = abs(rp.v.y);
rp.d.y = 0;
}
//printf("Final vx %f, vy %f, dx %f, dy %f, m %f\n\n\n", rp.v.x, rp.v.y, rp.d.x, rp.d.y, rp.m);


}

//-------------------logdata---------------------//
void log_data(prock rp, fvec & dfinal)
{
dfinal.v.push_back(rp.v);
dfinal.d.push_back(rp.d);
}


//----------------------Math----------------------------//
void addvec(vec & vf, vec v1, vec v2)
{
vf.x = v1.x + v2.x;
vf.y = v1.y + v2.y;
}

void subvec(vec & vf, vec v1, vec v2)
{
vf.x = v1.x - v2.x;
vf.y = v1.y - v2.y;
}

float dotprod(vec v1, vec v2)
{
float vs = v1.x*v2.x + v1.y*v2.y;
return vs;
}

void normalize(vec & vf, vec v1)
{

float hhh33 = sqrt(pow(v1.x,2)+ pow(v1.y,2));
vf.x = v1.x/hhh33;
vf.y = v1.y/hhh33;
}

float mag(vec v1)
{
float vs = sqrt(pow(v1.x,2)+ pow(v1.y,2));
return vs;
}

//------------------setters------------------//

void set_thrust(float temp){
thrust = temp;
thcurv = false;
}
void set_stmass(float temp){
stmass = temp;
}
void set_dragCd(float temp){
dragCd = temp;
}
void set_Xarea(float temp){
Xarea = temp;
}
void set_wind(float temp){
wind = temp;
}

float get_btime()
{
return btime;
}

void deploy_Chute(){
Xarea = para;
set_dragCd(1.36);
par = false;
}







































//end
