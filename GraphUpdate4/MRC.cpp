#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include "MRC.h"
#include "Point.h"
using namespace std;

void MRC::EMRead(string file)
{
	fstream inputFile(file.c_str(), ios::in|ios::binary);
	if(!inputFile)
	{
		cout<<"Can not open binary input file "<<file<<endl;
		exit(1);
	}

	inputFile.read(reinterpret_cast<char *>(&nx), sizeof(nx));
	inputFile.read(reinterpret_cast<char *>(&ny), sizeof(ny));
	inputFile.read(reinterpret_cast<char *>(&nz), sizeof(nz));
	inputFile.read(reinterpret_cast<char *>(&mode), sizeof(mode));
	inputFile.read(reinterpret_cast<char *>(&nxStart), sizeof(nxStart));
	inputFile.read(reinterpret_cast<char *>(&nyStart), sizeof(nyStart));
	inputFile.read(reinterpret_cast<char *>(&nzStart), sizeof(nzStart));
	inputFile.read(reinterpret_cast<char *>(&mx), sizeof(mx));
	inputFile.read(reinterpret_cast<char *>(&my), sizeof(my));
	inputFile.read(reinterpret_cast<char *>(&mz), sizeof(mz));
	inputFile.read(reinterpret_cast<char *>(&xLength), sizeof(xLength));
	inputFile.read(reinterpret_cast<char *>(&yLength), sizeof(yLength));
	inputFile.read(reinterpret_cast<char *>(&zLength), sizeof(zLength));
	inputFile.read(reinterpret_cast<char *>(&alpha), sizeof(alpha));
	inputFile.read(reinterpret_cast<char *>(&beta), sizeof(beta));
	inputFile.read(reinterpret_cast<char *>(&gamma), sizeof(gamma));
	inputFile.read(reinterpret_cast<char *>(&mapc), sizeof(mapc));
	inputFile.read(reinterpret_cast<char *>(&mapr), sizeof(mapr));
	inputFile.read(reinterpret_cast<char *>(&maps), sizeof(maps));
	inputFile.read(reinterpret_cast<char *>(&dMin), sizeof(dMin));
	inputFile.read(reinterpret_cast<char *>(&dMax), sizeof(dMax));
	inputFile.read(reinterpret_cast<char *>(&dMean), sizeof(dMean));
	inputFile.read(reinterpret_cast<char *>(&ispg), sizeof(ispg));
	inputFile.read(reinterpret_cast<char *>(&nsymbt), sizeof(nsymbt));
	inputFile.read(reinterpret_cast<char *>(extra), 25*sizeof(int));
	inputFile.read(reinterpret_cast<char *>(&xOrigin), sizeof(xOrigin));
	inputFile.read(reinterpret_cast<char *>(&yOrigin), sizeof(yOrigin));
	inputFile.read(reinterpret_cast<char *>(&zOrigin), sizeof(zOrigin));
	inputFile.read(reinterpret_cast<char *>(map), 4*sizeof(char));
	inputFile.read(reinterpret_cast<char *>(&machineStamp), sizeof(machineStamp));
	inputFile.read(reinterpret_cast<char *>(&rms), sizeof(rms));
	inputFile.read(reinterpret_cast<char *>(&nlabl), sizeof(nlabl));
	inputFile.read(reinterpret_cast<char *>(label), 10*80*sizeof(char));

	cube = new float **[nx];

	for(int i = 0; i < nx; i++)
	{
		cube[i] = new float *[ny];
		for(int j = 0; j < ny; j++)
			cube[i][j] = new float [nz];
	}

	for(int i = 0; i < nz; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nx; k++)
				inputFile.read(reinterpret_cast<char *>(&cube[k][j][i]), sizeof(float));

	inputFile.close();
}
void MRC::read(string file)
{
	fstream inputFile(file.c_str(), ios::in|ios::binary);
	if(!inputFile)
	{
		cout<<"Can not open binary input file "<<file<<endl;
		exit(1);
	}

	inputFile.read(reinterpret_cast<char *>(&nx), sizeof(nx));
	inputFile.read(reinterpret_cast<char *>(&ny), sizeof(ny));
	inputFile.read(reinterpret_cast<char *>(&nz), sizeof(nz));
	inputFile.read(reinterpret_cast<char *>(&mode), sizeof(mode));
	inputFile.read(reinterpret_cast<char *>(&nxStart), sizeof(nxStart));
	inputFile.read(reinterpret_cast<char *>(&nyStart), sizeof(nyStart));
	inputFile.read(reinterpret_cast<char *>(&nzStart), sizeof(nzStart));
	inputFile.read(reinterpret_cast<char *>(&mx), sizeof(mx));
	inputFile.read(reinterpret_cast<char *>(&my), sizeof(my));
	inputFile.read(reinterpret_cast<char *>(&mz), sizeof(mz));
	inputFile.read(reinterpret_cast<char *>(&xLength), sizeof(xLength));
	inputFile.read(reinterpret_cast<char *>(&yLength), sizeof(yLength));
	inputFile.read(reinterpret_cast<char *>(&zLength), sizeof(zLength));
	inputFile.read(reinterpret_cast<char *>(&alpha), sizeof(alpha));
	inputFile.read(reinterpret_cast<char *>(&beta), sizeof(beta));
	inputFile.read(reinterpret_cast<char *>(&gamma), sizeof(gamma));
	inputFile.read(reinterpret_cast<char *>(&mapc), sizeof(mapc));
	inputFile.read(reinterpret_cast<char *>(&mapr), sizeof(mapr));
	inputFile.read(reinterpret_cast<char *>(&maps), sizeof(maps));
	inputFile.read(reinterpret_cast<char *>(&dMin), sizeof(dMin));
	inputFile.read(reinterpret_cast<char *>(&dMax), sizeof(dMax));
	inputFile.read(reinterpret_cast<char *>(&dMean), sizeof(dMean));
	inputFile.read(reinterpret_cast<char *>(&ispg), sizeof(ispg));
	inputFile.read(reinterpret_cast<char *>(&nsymbt), sizeof(nsymbt));
	inputFile.read(reinterpret_cast<char *>(extra), 25*sizeof(int));
	inputFile.read(reinterpret_cast<char *>(&xOrigin), sizeof(xOrigin));
	inputFile.read(reinterpret_cast<char *>(&yOrigin), sizeof(yOrigin));
	inputFile.read(reinterpret_cast<char *>(&zOrigin), sizeof(zOrigin));
	inputFile.read(reinterpret_cast<char *>(map), 4*sizeof(char));
	inputFile.read(reinterpret_cast<char *>(&machineStamp), sizeof(machineStamp));
	inputFile.read(reinterpret_cast<char *>(&rms), sizeof(rms));
	inputFile.read(reinterpret_cast<char *>(&nlabl), sizeof(nlabl));
	inputFile.read(reinterpret_cast<char *>(label), 10*80*sizeof(char));

	cube = new float **[nx];

	for(int i = 0; i < nx; i++)
	{
		cube[i] = new float *[ny];
		for(int j = 0; j < ny; j++)
			cube[i][j] = new float [nz];
	}

	for(int i = 0; i < nz; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nx; k++)
				inputFile.read(reinterpret_cast<char *>(&cube[k][j][i]), sizeof(float));

	inputFile.close();
}

vector<Point> MRC::getPoints() const
{
	double apixX, apixY, apixZ;
	apixX = xLength/mx;
	apixY = yLength/my;
	apixZ = zLength/mz;

	vector<Point> pointContainer;

	for(int i = 0; i < nz; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nx; k++)
			{
				if(cube[k][j][i] > 0)
				{
					Point point(k*apixX+xOrigin, j*apixY+yOrigin, i*apixZ+zOrigin);
					pointContainer.push_back(point);
				}
			}

	return pointContainer;

}

void MRC::pdb(ofstream &file)
{
	double apixX, apixY, apixZ;
	apixX = xLength/mx;
	apixY = yLength/my;
	apixZ = zLength/mz;

	cout<<"ApixX: "<<apixX<<" ApixY: "<<apixY<<" ApixZ: "<<apixZ<<endl;

	int index = 0;
	int pointNum = 0;

	for(int i = 0; i < nz; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nx; k++)
			{
				if(cube[k][j][i] > 0)
				{
					Point point;
					point.set(k*apixX+xOrigin, j*apixY+yOrigin, i*apixZ+zOrigin);
					point.save(file, index, index);
					pointNum++;
				}
			}

	cout<<"There are "<<pointNum<<" points ..."<<endl;
}

void MRC::pdb(string str)
{
	ofstream file(str.c_str());

	double apixX, apixY, apixZ;
	apixX = xLength/mx;
	apixY = yLength/my;
	apixZ = zLength/mz;

	cout<<"ApixX: "<<apixX<<" ApixY: "<<apixY<<" ApixZ: "<<apixZ<<endl;

	int index = 0;
	int pointNum = 0;

	for(int i = 0; i < nz; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nx; k++)
			{
				if(cube[k][j][i] > 0)
				{
					Point point;
					point.set(k*apixX+xOrigin, j*apixY+yOrigin, i*apixZ+zOrigin);
					point.save(file, index, index);
					pointNum++;
				}
			}

	file.close();

	cout<<"Save points in density map to "<<str<<" with pdb format ..."<<endl;
}

void MRC::save(string file)
{
	fstream outputFile(file.c_str(), ios::out|ios::binary);
	if(!outputFile)
	{
		cout<<"Can not open binary output file "<<file<<endl;
		exit(1);
	}

	outputFile.write(reinterpret_cast<char *>(&nx), sizeof(nx));
	outputFile.write(reinterpret_cast<char *>(&ny), sizeof(ny));
	outputFile.write(reinterpret_cast<char *>(&nz), sizeof(nz));
	outputFile.write(reinterpret_cast<char *>(&mode), sizeof(mode));
	outputFile.write(reinterpret_cast<char *>(&nxStart), sizeof(nxStart));
	outputFile.write(reinterpret_cast<char *>(&nyStart), sizeof(nyStart));
	outputFile.write(reinterpret_cast<char *>(&nzStart), sizeof(nzStart));
	outputFile.write(reinterpret_cast<char *>(&mx), sizeof(mx));
	outputFile.write(reinterpret_cast<char *>(&my), sizeof(my));
	outputFile.write(reinterpret_cast<char *>(&mz), sizeof(mz));
	outputFile.write(reinterpret_cast<char *>(&xLength), sizeof(xLength));
	outputFile.write(reinterpret_cast<char *>(&yLength), sizeof(yLength));
	outputFile.write(reinterpret_cast<char *>(&zLength), sizeof(zLength));
	outputFile.write(reinterpret_cast<char *>(&alpha), sizeof(alpha));
	outputFile.write(reinterpret_cast<char *>(&beta), sizeof(beta));
	outputFile.write(reinterpret_cast<char *>(&gamma), sizeof(gamma));
	outputFile.write(reinterpret_cast<char *>(&mapc), sizeof(mapc));
	outputFile.write(reinterpret_cast<char *>(&mapr), sizeof(mapr));
	outputFile.write(reinterpret_cast<char *>(&maps), sizeof(maps));
	outputFile.write(reinterpret_cast<char *>(&dMin), sizeof(dMin));
	outputFile.write(reinterpret_cast<char *>(&dMax), sizeof(dMax));
	outputFile.write(reinterpret_cast<char *>(&dMean), sizeof(dMean));
	outputFile.write(reinterpret_cast<char *>(&ispg), sizeof(ispg));
	outputFile.write(reinterpret_cast<char *>(&nsymbt), sizeof(nsymbt));
	outputFile.write(reinterpret_cast<char *>(extra), 25*sizeof(int));
	outputFile.write(reinterpret_cast<char *>(&xOrigin), sizeof(xOrigin));
	outputFile.write(reinterpret_cast<char *>(&yOrigin), sizeof(yOrigin));
	outputFile.write(reinterpret_cast<char *>(&zOrigin), sizeof(zOrigin));
	outputFile.write(reinterpret_cast<char *>(map), 4*sizeof(char));
	outputFile.write(reinterpret_cast<char *>(&machineStamp), sizeof(machineStamp));
	outputFile.write(reinterpret_cast<char *>(&rms), sizeof(rms));
	outputFile.write(reinterpret_cast<char *>(&nlabl), sizeof(nlabl));
	outputFile.write(reinterpret_cast<char *>(label), 10*80*sizeof(char));

	for(int i = 0; i < nz; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nx; k++)
				outputFile.write(reinterpret_cast<char *>(&cube[k][j][i]), sizeof(float));

	outputFile.close();
}

void MRC::surface(double cutOff)
{
	float array[nx][ny][nz];

	for(int i = 0; i < nx; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nz; k++)
				array[i][j][k] = 0;

	for(int i = 0; i < nx; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nz; k++)
			{
				if(isSurface(i, j, k, cutOff))
					array[i][j][k] = cube[i][j][k];
			}

	for(int i = 0; i < nx; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nz; k++)
			{
				cube[i][j][k] = array[i][j][k];
			}
}

bool MRC::isSurface(int iTemp, int jTemp, int kTemp, double cutOff)
{
	if(iTemp+1 >= nx) return false;
	if(iTemp-1 < 0) return false;
	if(jTemp+1 >= ny) return false;
	if(jTemp-1 < 0) return false;
	if(kTemp+1 >= nz) return false;
	if(kTemp-1 < 0) return false;

	if(cube[iTemp][jTemp][kTemp] > cutOff && (cube[iTemp+1][jTemp][kTemp] <= cutOff || cube[iTemp][jTemp+1][kTemp] <= cutOff || cube[iTemp][jTemp][kTemp+1] <= cutOff || cube[iTemp-1][jTemp][kTemp] <= cutOff || cube[iTemp][jTemp-1][kTemp] <= cutOff || cube[iTemp][jTemp][kTemp-1] <= cutOff))
		return true;
	else return false;
}

MRC::MRC(MRC &right)
{
	nx = right.nx;
	ny = right.ny;
	nz = right.nz;
	mode = right.mode;
	nxStart = right.nxStart;
	nyStart = right.nyStart;
	nzStart = right.nzStart;
	mx = right.mx;
	my = right.my;
	mz = right.mz;
	xLength = right.xLength;
	yLength = right.yLength;
	zLength = right.zLength;
	alpha = right.alpha;
	beta = right.beta;
	gamma = right.gamma;
	mapc = right.mapc;
	mapr = right.mapr;
	maps = right.maps;
	dMin = right.dMin;
	dMax = right.dMax;
	dMean = right.dMean;
	ispg = right.ispg;
	nsymbt = right.nsymbt;
	for(int i = 0; i < 25; i++)
		extra[i] = right.extra[i];
	xOrigin = right.xOrigin;
	yOrigin = right.yOrigin;
	zOrigin = right.zOrigin;
	for(int i = 0; i < 4; i++)
		map[i] = right.map[i];
	machineStamp = right.machineStamp;
	rms = right.rms;
	nlabl = right.nlabl;
	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 80; j++)
			label[i][j] = right.label[i][j];


	cube = new float **[nx];

	for(int i = 0; i < nx; i++)
	{
		cube[i] = new float *[ny];
		for(int j = 0; j < ny; j++)
			cube[i][j] = new float [nz];
	}

	for(int i = 0; i < nx; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nz; k++)
				cube[i][j][k] = right.cube[i][j][k];
}

MRC MRC::operator=(MRC &right)
{
	nx = right.nx;
	ny = right.ny;
	nz = right.nz;
	mode = right.mode;
	nxStart = right.nxStart;
	nyStart = right.nyStart;
	nzStart = right.nzStart;
	mx = right.mx;
	my = right.my;
	mz = right.mz;
	xLength = right.xLength;
	yLength = right.yLength;
	zLength = right.zLength;
	alpha = right.alpha;
	beta = right.beta;
	gamma = right.gamma;
	mapc = right.mapc;
	mapr = right.mapr;
	maps = right.maps;
	dMin = right.dMin;
	dMax = right.dMax;
	dMean = right.dMean;
	ispg = right.ispg;
	nsymbt = right.nsymbt;
	for(int i = 0; i < 25; i++)
		extra[i] = right.extra[i];
	xOrigin = right.xOrigin;
	yOrigin = right.yOrigin;
	zOrigin = right.zOrigin;
	for(int i = 0; i < 4; i++)
		map[i] = right.map[i];
	machineStamp = right.machineStamp;
	rms = right.rms;
	nlabl = right.nlabl;
	for(int i = 0; i < 10; i++)
		for(int j = 0; j < 80; j++)
			label[i][j] = right.label[i][j];


	cube = new float **[nx];

	for(int i = 0; i < nx; i++)
	{
		cube[i] = new float *[ny];
		for(int j = 0; j < ny; j++)
			cube[i][j] = new float [nz];
	}

	for(int i = 0; i < nx; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nz; k++)
				cube[i][j][k] = right.cube[i][j][k];

	return *this;
}

void MRC::show()
{
	cout<<"nx, ny, nz: "<<nx<<","<<ny<<","<<nz<<endl;
	cout<<"mode: "<<mode<<endl;
	cout<<"nxStart, nyStart, nzStart: "<<nxStart<<","<<nyStart<<","<<nzStart<<endl;
	cout<<"mx, my, mz: "<<mx<<","<<my<<","<<mz<<endl;
	cout<<"xLength, yLength, zLength: "<<xLength<<","<<yLength<<","<<zLength<<endl;
	cout<<"alpha, beta, gamma: "<<alpha<<","<<beta<<","<<gamma<<endl;
	cout<<"mapc, mapr, maps: "<<mapc<<","<<mapr<<","<<maps<<endl;
	cout<<"dMin, dMax, dMean: "<<dMin<<","<<dMax<<","<<dMean<<endl;
	cout<<"ispg: "<<ispg<<endl;
	cout<<"nsymbt: "<<nsymbt<<endl;
	for(int i = 0; i < 25; i ++)
		cout<<extra[i]<<" ";
	cout<<endl;
	cout<<"xOrigin, yOrigin, zOrigin: "<<xOrigin<<","<<yOrigin<<","<<zOrigin<<endl;
	cout<<"map: "<<map<<endl;
	cout<<"machineStamp: "<<machineStamp<<endl;
	cout<<"rms: "<<rms<<endl;
	cout<<"nlabl: "<<nlabl<<endl;
	for(int i = 0; i < 10; i++)
		cout<<label[i]<<endl;
	cout<<"-------------------------------------------"<<endl;
	for(int i = 0; i < nx; i++)
		for(int j = 0; j < ny; j++)
		{
			for(int k = 0; k < nz; k++)
			{
				if(cube[i][j][k] != 0)
					cout<<"Density: "<<i<<" "<<j<<" "<<k<<" "<<cube[i][j][k]<<" ";
			}
			cout<<endl;
		}
	cout<<endl;
}

void MRC::output(string file)
{
	ofstream outputFile(file.c_str());

	for(int i = 0; i < nx; i++)
		for(int j = 0; j < ny; j++)
			for(int k = 0; k < nz; k++)
			{
				if(cube[i][j][k] > 0)
					outputFile<<setw(10)<<i<<setw(10)<<j<<setw(10)<<k<<setw(20)<<cube[i][j][k]<<endl;
			}

	outputFile.close();
}

MRC::~MRC()
{
	for(int i = 0; i < nx; i++)
		for(int j = 0; j < ny; j++)
		{
			delete [] cube[i][j];
		}

	for(int i = 0; i < nx; i++)
		delete [] cube[i];

	delete [] cube;
}
