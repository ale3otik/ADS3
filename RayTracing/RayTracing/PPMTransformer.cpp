//  PPMTransformer.cpp
//  RayTracing
//
//  Created by Alex on 15.04.16.
//  Copyright © 2016 Alex. All rights reserved.

#include "PPMTransformer.hpp"
#include <cassert>
typedef long long i64;
void PPMTransformer::transformToPPM(std::vector<std::vector<Color> > data , const std::string & fname) {
    assert(data.size() > 0);
    i64 ysize = data.size();
    i64 xsize = data[0].size();
    
    std::ofstream file(fname);
    assert(file.is_open());
    
    
    file << "P3 \n";
    file << xsize << " " << ysize << "\n";
    file << "255 \n";
    
    for(const auto & line : data) {
        for(const auto & pix : line) {
            file << pix.r << " " << pix.g << " " << pix.b <<"\n";
        }
    }
    file << "\n";
    file.close();
}

std::vector<std::shared_ptr<Shape> > PPMTransformer::getShapesFromFile(const std::string & fname) {
    std::ifstream file(fname);
    assert(file.is_open());
    
    int qnt;
    file >> qnt;
    assert(qnt >= 0);
    std::vector<std::shared_ptr<Shape> > result(qnt);

    for(int i = 0 ; i < qnt; ++i) {
        int id;
        file >> id;
        Color clr;
        long double mirror_rate;
        file >> clr.r >> clr.g >> clr.b;
        file >> mirror_rate;
        clr.r = std::max(5,clr.r);
        clr.g = std::max(5,clr.g);
        clr.b = std::max(5,clr.b);
        if(id == 1) {  // triangle
            crd vertices[3];
            for(int j = 0; j < 3; ++j) {
                file >> vertices[j].x >> vertices[j].y >> vertices[j].z;
            }
            
            std::shared_ptr<Triangle> t(new Triangle(vertices));
            t->setColor(clr);
            t->setMirrorRate(mirror_rate);
            result[i] = t;
        } else if(id == 2) {
            crd center;
            double radius;
            file >> center.x >> center.y >> center.z;
            file >> radius;
            std::shared_ptr<Sphere> s(new Sphere(center,radius));
            s->setColor(clr);
            s->setMirrorRate(mirror_rate);
            result[i] = s;
        } else {
            return result;
        }
    }
    
    file.close();
    return result;
}

std::vector<std::shared_ptr<Light> > PPMTransformer::getLightFromFile(const std::string & fname) {
    std::ifstream file(fname);
    assert(file.is_open());
    
    int qnt;
    file >> qnt;
    assert(qnt >= 0);
    std::vector<std::shared_ptr<Light> > result(qnt);
    
    for(int i = 0 ; i < qnt; ++i) {
        crd pos;
        int intense;
        file >> pos.x >> pos.y >> pos.z;
        file >> intense;
        std::shared_ptr<Light> l(new  Light(pos,intense));
        result[i] = l;
    }
    
    file.close();
    return result;
}

std::vector<std::shared_ptr<Shape> > PPMTransformer::scanDataFromASCISTL(const std::string & fname) {
    std::vector<std::shared_ptr<Shape> > shapes;
    std::ifstream file(fname);
    assert(file.is_open());
    std::string s;
    file >> s;
    if(s != "solid") {
        assert(0);
    }
    file >> s >> s;
    while(s != "endsolid") {
        file >> s;
        crd norm;
        file >> norm.x >> norm.y >> norm.z;
        file >> s >> s;
        crd v[3];
        for(int i = 0;i < 3;++i) {
            file >> s;
            file >> v[i].x >> v[i].z >> v[i].y;
//            v[i].x *= 1e2; v[i].y *= 1e2; v[i].z *= 1e2;
        }
        std::shared_ptr<Shape> tr = std::shared_ptr<Shape>(new Triangle(v));
        tr->setColor(Color(100,100,100));
        shapes.push_back(tr);
        file >> s >> s >> s;
    }
    file.close();
    return shapes;
}
