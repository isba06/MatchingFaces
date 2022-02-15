#include <iostream>
#include <dlib/matrix.h>
#include <vector>
#include <boost/algorithm/string.hpp>

struct Person {
    std::string id;
    std::string name;
    dlib::matrix<float, 128, 1> encoding;
};

float ColumnMatrix_EuclideanDistance(dlib::matrix<float> DB_EncodingFaceMatrix, dlib::matrix<float> EncodingFaceVector) {
    dlib::matrix<float, 0, 1> euclideanNorm;
    euclideanNorm.set_size(DB_EncodingFaceMatrix.nc());
    for (int i = 0; i < DB_EncodingFaceMatrix.nc(); i++) {
        euclideanNorm(i, 0) = length(colm(DB_EncodingFaceMatrix, i) - EncodingFaceVector);
    }

    if (min(euclideanNorm) < 0.5)
        return min_point(euclideanNorm).y();
    else return -1;
}
int main()
{
    setlocale(LC_CTYPE, "");
    std::ifstream in("C:\\Users\\wwwis\\Desktop\\Job\\2rus_face_bd.txt", ios::in);

    if (!in.is_open()) {
        std::cout << "DB file is not open";
        return 0;
    }
    dlib::matrix<float, 128, 1> bd_face_encoding, face_enc;
    std::vector<string> strVec;
    std::vector<Person> personBD;
    std::string line;
    Person person{};
    while (std::getline(in, line)) {
        if (!line.empty()) {
            boost::split(strVec, line, boost::is_any_of(","));

            person.id = strVec[0];
            person.name = strVec[1];
            std::vector<std::string> encodingVector;
            boost::split(encodingVector, strVec[2], boost::is_any_of(" "));
            dlib::matrix<float, 128, 1> face_encoding;
            for (long raw = 0; raw < face_encoding.nr(); raw++)
                face_encoding(raw, 0) = std::stof(encodingVector[raw]);
            person.encoding = face_encoding;
            personBD.push_back(person);
        }
    }
    dlib::matrix<float, 0, 0>faceEncodingMatrix, fc;
    faceEncodingMatrix.set_size(128, personBD.size());
    std::cout << faceEncodingMatrix.nc() << std::endl;
    std::cout << faceEncodingMatrix.nr() << std::endl;
    for (long col = 0; col < personBD.size(); col++) {
        dlib::set_colm(faceEncodingMatrix, col) = personBD[col].encoding;
    }
    std::ifstream F("C:\\Users\\wwwis\\Desktop\\Job\\test.txt", ios::in);
    dlib::matrix<float, 128, 1> encodingFace;

    if (!in.is_open()) {
        std::cout << "test file is not open";
        return 0;
    }
    int i = 0;
    while (!F.eof())
    {
        F >> encodingFace(i++);
    }
    std::cout << faceEncodingMatrix << std::endl;
    std::cout << personBD[ColumnMatrix_EuclideanDistance(faceEncodingMatrix, encodingFace)].id << std::endl;
    std::cout << personBD[ColumnMatrix_EuclideanDistance(faceEncodingMatrix, encodingFace)].name << std::endl;
    return 0;
}
