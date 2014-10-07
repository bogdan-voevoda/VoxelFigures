#include "geometryengine.h"

GeometryEngine::GeometryEngine(MainWindow *parent): parent(parent) {
    field = new Matrix3D(350,350,350);
}

Matrix3D* GeometryEngine::getMatrix() {
    return this->field;
}

void GeometryEngine::init() {
    initializeGLFunctions();
    glGenBuffers(2, vboIds);
}

void GeometryEngine::initGeometry() {
    if (field) {
        unsigned int size = vertices.size() * sizeof(GLfloat);
        glBindBuffer(GL_ARRAY_BUFFER, vboIds[0] );
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, &vertices[0]);

        size = indices.size() * sizeof(GLuint);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1] );
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, &indices[0]);
    }
}

void GeometryEngine::drawGeometry(QGLShaderProgram *program) {
    if ( field ) {
        glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

        int vertexLocation = program->attributeLocation("a_position");
        program->enableAttributeArray(vertexLocation);
        glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
        program->setUniformValue("color", 0,1,0,0);
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_INT, 0);
    }

    short height = 120;
    glLineWidth(1.5f);

    program->setUniformValue("color", 1,0,0,0);
    glBegin(GL_LINES);
        glVertex3f( height/2,  0.0f,  0.0f );
        glVertex3f(-height/2,  0.0f,  0.0f );
    glEnd();

    program->setUniformValue("color", 0.2f,0.8f,0.2f,0.0f);
    glBegin(GL_LINES);
        glVertex3f( 0.0f,  height/2,  0.0f );
        glVertex3f( 0.0f, -height/2,  0.0f );
    glEnd();

    program->setUniformValue("color", 0,0,1,0);
    glBegin(GL_LINES);
        glVertex3f( 0.0f,  0.0f,  height/2 );
        glVertex3f( 0.0f,  0.0f, -height/2 );
    glEnd();
}

void GeometryEngine::getVertices(Matrix3D *field,
                                 std::vector <GLfloat> &vertices,
                                 std::vector <GLuint> &indices) {
    int counterIndex = 0;
    int step = 8;
    indices.clear();
    vertices.clear();

    int xBorder = field->maxX+field->heightShift+1;
    for(int x = field->minX+field->heightShift; x < xBorder; x++) {
        int yBorder = field->maxY+field->widthShift+1;
        for(int y = field->minY+field->widthShift; y < yBorder; y++) {
            int zBorder = field->maxZ+field->depthShift+1;
            for(int z = field->minZ+field->depthShift; z < zBorder; z++) {
                if( field->get(x, y, z) ) {
                    if ( !field->get(x-1, y, z) ||
                         !field->get(x+1, y, z) ||
                         !field->get(x, y+1, z) ||
                         !field->get(x, y-1, z) ||
                         !field->get(x, y, z+1) ||
                         !field->get(x, y, z-1) ){

                        addVertices(x-field->heightShift,
                                    y-field->widthShift,
                                    z-field->depthShift,
                                    counterIndex,
                                    vertices,
                                    indices);

                        counterIndex += step;
                    }
                }
            }
        }
    }
}

void GeometryEngine::updateVertices() {
    getVertices(this->field, vertices, indices);
    initGeometry();
    parent->updateGLViews();
}
void GeometryEngine::addVertices(float x, float y, float z, int indexer,
                                 std::vector<GLfloat> &vertices,
                                 std::vector<GLuint> &indices) {
    x -= 0.15f;
    y -= 0.15f;
    z -= 0.15f;
    float size = 0.3f;

    vertices.push_back(x);      //0
    vertices.push_back(y);
    vertices.push_back(z);

    vertices.push_back(x+size); //1
    vertices.push_back(y);
    vertices.push_back(z);

    vertices.push_back(x+size); //2
    vertices.push_back(y+size);
    vertices.push_back(z);

    vertices.push_back(x);      //3
    vertices.push_back(y+size);
    vertices.push_back(z);

    vertices.push_back(x);      //4
    vertices.push_back(y);
    vertices.push_back(z+size);

    vertices.push_back(x+size); //5
    vertices.push_back(y);
    vertices.push_back(z+size);

    vertices.push_back(x+size); //6
    vertices.push_back(y+size);
    vertices.push_back(z+size);

    vertices.push_back(x);      //7
    vertices.push_back(y+size);
    vertices.push_back(z+size);


    indices.push_back(4+indexer);
    indices.push_back(7+indexer);
    indices.push_back(6+indexer);
    indices.push_back(5+indexer);

    indices.push_back(3+indexer);
    indices.push_back(0+indexer);
    indices.push_back(1+indexer);
    indices.push_back(2+indexer);

    indices.push_back(0+indexer);
    indices.push_back(4+indexer);
    indices.push_back(5+indexer);
    indices.push_back(1+indexer);

    indices.push_back(2+indexer);
    indices.push_back(6+indexer);
    indices.push_back(7+indexer);
    indices.push_back(3+indexer);

    indices.push_back(3+indexer);
    indices.push_back(7+indexer);
    indices.push_back(4+indexer);
    indices.push_back(0+indexer);

    indices.push_back(1+indexer);
    indices.push_back(5+indexer);
    indices.push_back(6+indexer);
    indices.push_back(2+indexer);
}

void GeometryEngine::update_Geometry(MatrixData *matrixData) {
    short index = matrixData->strNumber;

    if ( matrixData->operation == -1 && matrixData->figure == -1 ) {
        field->clear();
        field->setDefaultBorders();

        QMap <short, MatrixData*>::iterator it = matData.begin();
        for ( ; it != matData.end() ; ++it) {
            delete it.value();
        }
        matData.clear();
        qDebug() << "Good case";

    } else if ( matData.contains(index) && !isComparable(matrixData, matData[index]) ) {
        matData[index]->changeData(matrixData);
        field->clear();
        field->setDefaultBorders();
        for ( short i = 0; i < matData.size(); ++i ) {
            field->updateMatrix(matData[i]);
        }
        qDebug() << "Worst case";

    } else if ( !matData.contains(index) ) {
        MatrixData *data = new MatrixData(matrixData->operation,
                                          matrixData->figure,
                                          matrixData->x1,
                                          matrixData->x2,
                                          matrixData->y1,
                                          matrixData->y2,
                                          matrixData->z1,
                                          matrixData->z2,
                                          matrixData->strNumber);
        matData.insert(index, data);
        field->updateMatrix(matData[index]);
        qDebug() << "Medium case";
    } else if ( matData.contains(index) && isComparable(matrixData, matData[index])) {
        qDebug() << "Best case";
    }

    getVertices(field, vertices, indices);
    initGeometry();
    parent->updateGLViews();
}

bool GeometryEngine::isComparable(MatrixData *arr, MatrixData* data) {
    if ( arr->operation == data->operation &&
         arr->figure == data->figure &&
         arr->x1 == data->x1 &&
         arr->x2 == data->x2 &&
         arr->y1 == data->y1 &&
         arr->y2 == data->y2 &&
         arr->z1 == data->z1 &&
         arr->z2 == data->z2 &&
         arr->strNumber == data->strNumber) {
        return true;
    } else { return false; }
}

GeometryEngine::~GeometryEngine() {
    glDeleteBuffers(2, vboIds);
    if (field) { delete field; }
}
