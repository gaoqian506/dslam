
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include <eigen3/Eigen/Core>
#include <Eigen/StdVector>

#include "g2o/types/slam3d/vertex_se3.h"
#include "g2o/types/slam3d/edge_se3.h"
#include "g2o/stuff/sampler.h"
#include "g2o/stuff/command_args.h"
#include "g2o/core/factory.h"

using namespace std;
using namespace g2o;

int main() {

	int nodesPerLevel = 50;
	int numLaps = 50;
	bool randomSeed = true;
	double radius = 100;


	std::vector<double> noiseTranslation;
	std::vector<double> noiseRotation;


	if (noiseTranslation.size() == 0) {
		cerr << "using default noise for the translation" << endl;
		noiseTranslation.push_back(0.01);
		noiseTranslation.push_back(0.01);
		noiseTranslation.push_back(0.01);
	}
	cerr << "Noise for the translation:";
	for (size_t i = 0; i < noiseTranslation.size(); ++i)
		cerr << " " << noiseTranslation[i];
	cerr << endl;
	if (noiseRotation.size() == 0) {
	cerr << "using default noise for the rotation" << endl;
		noiseRotation.push_back(0.005);
		noiseRotation.push_back(0.005);
		noiseRotation.push_back(0.005);
	}
	cerr << "Noise for the rotation:";
	for (size_t i = 0; i < noiseRotation.size(); ++i)
		cerr << " " << noiseRotation[i];
	cerr << endl;

	Eigen::Matrix3d transNoise = Eigen::Matrix3d::Zero();
	for (int i = 0; i < 3; ++i)
		transNoise(i, i) = std::pow(noiseTranslation[i], 2);

	Eigen::Matrix3d rotNoise = Eigen::Matrix3d::Zero();
	for (int i = 0; i < 3; ++i)
		rotNoise(i, i) = std::pow(noiseRotation[i], 2);

	Eigen::Matrix<double, 6, 6> information = Eigen::Matrix<double, 6, 6>::Zero();

	information.block<3,3>(0,0) = transNoise.inverse();
	information.block<3,3>(3,3) = rotNoise.inverse();

	vector<VertexSE3*> vertices;
	vector<EdgeSE3*> odometryEdges;	
	vector<EdgeSE3*> edges;	

	int id = 0;
	for (int f = 0; f < numLaps; ++f){
		for (int n = 0; n < nodesPerLevel; ++n) {
			VertexSE3* v = new VertexSE3;
			v->setId(id++);

			Eigen::AngleAxisd rotz(-M_PI + 2*n*M_PI / nodesPerLevel, Eigen::Vector3d::UnitZ());
			Eigen::AngleAxisd roty(-0.5*M_PI + id*M_PI / (numLaps * nodesPerLevel), Eigen::Vector3d::UnitY());
			Eigen::Matrix3d rot = (rotz * roty).toRotationMatrix();

			Eigen::Isometry3d t;
			t = rot;
			t.translation() = t.linear() * Eigen::Vector3d(radius, 0, 0);
			v->setEstimate(t);
			vertices.push_back(v);

		}
	}

	// generate odometry edges
	for (size_t i = 1; i < vertices.size(); ++i) {
		VertexSE3* prev = vertices[i-1];
		VertexSE3* cur  = vertices[i];
		Eigen::Isometry3d t = prev->estimate().inverse() * cur->estimate();
		EdgeSE3* e = new EdgeSE3;
		e->setVertex(0, prev);
		e->setVertex(1, cur);
		e->setMeasurement(t);
		e->setInformation(information);
		odometryEdges.push_back(e);
		edges.push_back(e);
	}

	// generate loop closure edges
	for (int f = 1; f < numLaps; ++f) {
		for (int nn = 0; nn < nodesPerLevel; ++nn) {
			VertexSE3* from = vertices[(f-1)*nodesPerLevel + nn];
			for (int n = -1; n <= 1; ++n) {
				if (f == numLaps-1 && n == 1)
				  continue;
				VertexSE3* to   = vertices[f*nodesPerLevel + nn + n];
				Eigen::Isometry3d t = from->estimate().inverse() * to->estimate();
				EdgeSE3* e = new EdgeSE3;
				e->setVertex(0, from);
				e->setVertex(1, to);
				e->setMeasurement(t);
				e->setInformation(information);
				edges.push_back(e);
			}
		}
	}

	GaussianSampler<Eigen::Vector3d, Eigen::Matrix3d> transSampler;
	transSampler.setDistribution(transNoise);
	GaussianSampler<Eigen::Vector3d, Eigen::Matrix3d> rotSampler;
	rotSampler.setDistribution(rotNoise);


	if (randomSeed) {
		std::random_device r;
		std::seed_seq seedSeq{r(), r(), r(), r(), r()};
		vector<int> seeds(2);
		seedSeq.generate(seeds.begin(), seeds.end());
		cerr << "using seeds:";
		for (size_t i = 0; i < seeds.size(); ++i)
			cerr << " " << seeds[i];
		cerr << endl;
		transSampler.seed(seeds[0]);
		rotSampler.seed(seeds[1]);
	}

	// noise for all the edges
	for (size_t i = 0; i < edges.size(); ++i) {
		EdgeSE3* e = edges[i];
		Eigen::Quaterniond gtQuat = (Eigen::Quaterniond)e->measurement().linear();
		Eigen::Vector3d gtTrans = e->measurement().translation();

		Eigen::Vector3d quatXYZ = rotSampler.generateSample();
		double qw = 1.0 - quatXYZ.norm();
		if (qw < 0) {
			qw = 0.;
			cerr << "x";
		}
		Eigen::Quaterniond rot(qw, quatXYZ.x(), quatXYZ.y(), quatXYZ.z());
		rot.normalize();
		Eigen::Vector3d trans = transSampler.generateSample();
		rot = gtQuat * rot;
		trans = gtTrans + trans;

		Eigen::Isometry3d noisyMeasurement = (Eigen::Isometry3d) rot;
		noisyMeasurement.translation() = trans;
		e->setMeasurement(noisyMeasurement);
	}

	// concatenate all the odometry constraints to compute the initial state
	for (size_t i =0; i < odometryEdges.size(); ++i) {
		EdgeSE3* e = edges[i];
		VertexSE3* from = static_cast<VertexSE3*>(e->vertex(0));
		VertexSE3* to = static_cast<VertexSE3*>(e->vertex(1));
		HyperGraph::VertexSet aux; aux.insert(from);
		e->initialEstimate(aux, to);
	}

	// write output
	ofstream fileOutputStream;
	if (outFilename != "-") {
		cerr << "Writing into " << outFilename << endl;
			fileOutputStream.open(outFilename.c_str());
		} else {
			cerr << "writing to stdout" << endl;
	}

	string vertexTag = Factory::instance()->tag(vertices[0]);
	string edgeTag = Factory::instance()->tag(edges[0]);

	ostream& fout = outFilename != "-" ? fileOutputStream : cout;
	for (size_t i = 0; i < vertices.size(); ++i) {
		VertexSE3* v = vertices[i];
		fout << vertexTag << " " << v->id() << " ";
		v->write(fout);
		fout << endl;
	}

	for (size_t i = 0; i < edges.size(); ++i) {
		EdgeSE3* e = edges[i];
		VertexSE3* from = static_cast<VertexSE3*>(e->vertex(0));
		VertexSE3* to = static_cast<VertexSE3*>(e->vertex(1));
		fout << edgeTag << " " << from->id() << " " << to->id() << " ";
		e->write(fout);
		fout << endl;
	}

	return 0;
}
