#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct dtp { float x, y; };

vector<dtp> k,lv,extr;

float x1 = -720; float x2 = 720; float p = 50;
float a = 10; float b = 0.1; float c = -10;


float calcfunc(float a, float b, float c, float x)
{
	//return a * pow(x, 2) + b * x + c;
	return a * sin(b*x) +c;
}

void makevector(float x1, float x2, int p, vector<dtp> &kk)
{
	dtp ck;
	float delta = (x2 - x1) / p;
	kk.clear();
	for (int i = 0; i <= p; i++)
	{
		ck.x = x1 + i * delta;
		ck.y = calcfunc(a, b, c, ck.x);
		kk.push_back(ck);
	}
}

float calcli(vector<dtp> kk, float x, int i)
{
	int l = kk.size();
	float res=1;
	for (int j = 0; j < l; j++)
	{
		if (j!=i) res = res * ((x - kk[j].x) / (kk[i].x - kk[j].x));
	}
	return res;
}

float lagrange(vector<dtp> kk,float x)
{
	float res = 0;
	int l= kk.size();
	for (int i = 0; i < l; i++)
	{
		res +=  (kk[i].y*calcli(kk,x,i));
	}
	return res;
}

void makelagvector(vector<dtp> kk, vector<dtp> &ll)
{
	int pp = p * 10;
	dtp ck;
	float delta = (x2 - x1) / (float)pp;
	ll.clear();
	for (int i = 0; i <= pp; i++)
	{
		ck.x = x1 + i * delta;
		ck.y = lagrange(kk, ck.x);
		ll.push_back(ck);
		printf("lv %d) %f; %f\n",i,ck.x,ck.y);
	}
}

void findextremes(vector<dtp> kk, vector<dtp> &ex)
{
	int l = kk.size();
	ex.clear();
	for (int i = 1; i < (l-1); i++)
	{
		if (((kk[i].y > kk[i - 1].y) && (kk[i].y > kk[i + 1].y)) ||
			((kk[i].y < kk[i - 1].y) && (kk[i].y < kk[i + 1].y)))
		{
			ex.push_back(kk[i]);
		}
	}
}

void writevector(vector<dtp> kk, string fn)
{
	ofstream fs;
	fs.open(fn,ios::out | ios::trunc);
	int l = kk.size();
	for (int i = 0; i < l; i++)
		fs << to_string(kk[i].x) << ";" << to_string(kk[i].y) << endl;
	fs.close();
}

float linterp(vector<dtp> k, float x)
{
	if (k.size() > 1)
	{
		int t=0;
		float cx = k[0].x;
		float nx1=k[0].x, nx2=k[1].x;
		float ny1 = 0, ny2 = 0;
		while ((nx1 < x) && (cx<k[k.size()-2].x))
		{
			cx = k[t].x;
			if ((cx >= nx1) && (cx < x))
			{
				nx1 = k[t].x; nx2 = k[t + 1].x;
				ny1 = k[t].y; ny2 = k[t + 1].y;
			}
			t++;
		}

		return ny1 + ((ny2 - ny1) / (nx2 - nx1))*(x-nx1);

	}
	else
	{
		return 0;
	}


}



int main()
{
	printf("Input precision:\n");
	string prec;
	getline(cin, prec);
	p = atoi(prec.c_str());
	makevector(x1,x2,p,k);
	makelagvector(k, lv);
	findextremes(k,extr);
	writevector(k,"vdata.csv");
	writevector(lv, "lvdata.csv");
	writevector(extr, "exdata.csv");
	for (int i = 0; i <= p; i++)
	{
		printf("%d) %f; %f;\n", i, k[i].x, k[i].y);
	}

	printf("Input interpolation value for X:\n");
	string tx;
	getline(cin, tx);
	float sx = atof(tx.c_str());
	float sy = linterp(k, sx);
	float sy2 = lagrange(k, sx);
	printf("Interpolated value: %f; lin=%f; lag=%f\n",sx,sy,sy2);

	system("pause");

}


