
#include <stdio.h>
#include <cstdlib>
#include <assert.h>

#define DEBUG

#define POISON 666

#define VECTOR_OK() if (OK() != 0) {dump(); assert(!"ok");}


enum Err {
    E_OK,                   // stack is valid;
    E_NULL_PTR_BUF,     // stack is pointed by NULL;
    E_INVALID_SIZE,       // size is invalid;
    E_INVALID_MAXSIZE,    // maxSize is invalid;
};

const char* errMessage[] = {
        "vector is OK",
        "buffer of vector is nullptr",
        "maxSize is 0",
        "size is more than maxSize",

};

using namespace std;

typedef int elem_t;

struct Vector {
private:
    elem_t *buf_;
    size_t size_;
    size_t maxSize_;

    int errNo_;
    int OK();
    void dump() const;
public:
    explicit Vector(int newMaxSize);
    ~Vector();
    elem_t & operator [](int index);
};



Vector::Vector(int newMaxSize)
        :
        buf_( (elem_t *)calloc(newMaxSize, sizeof(*buf_)) ),
        size_(0),
        maxSize_(newMaxSize),
        errNo_(E_OK) {


#ifdef DEBUG
    VECTOR_OK();
#else
    assert(buf_ != nullptr);
#endif
    assert(newMaxSize > 0);

}

Vector::~Vector() {

    free(buf_);
    buf_ = nullptr;
    size_ = POISON;
    maxSize_ = POISON;
}

elem_t & Vector::operator [](int index) {
#ifdef DEBUG
    VECTOR_OK();
#else
    assert(index >= 0);
    assert(index < size_);
#endif

    return buf_[index];
}

int Vector::OK () {
    if (buf_ == nullptr) {
        errNo_ = E_NULL_PTR_BUF;
        return E_NULL_PTR_BUF;
}

    if (maxSize_  == 0) {
        errNo_ = E_INVALID_MAXSIZE;
        return E_INVALID_MAXSIZE;
    }

    if (size_ > maxSize_) {
        errNo_ = E_INVALID_SIZE;
        return E_INVALID_SIZE;
    }

    return E_OK;
}

void Vector::dump () const {

    const char *filename = "dump.log";

    FILE *fdump = fopen (filename, "a+");

    if (fdump == nullptr) {
        printf ("Can't open file %s", filename);
        exit(1);
    }


    if (errNo_ == E_OK) {
        fprintf(fdump, "%s", errMessage[errNo_]);
    }
    else {
        fprintf(fdump, "!!!ERROR <%d>: %s !!!\n",
                errNo_,
                errMessage[errNo_]);
    }

    fprintf(fdump, " [%p] {\n"
                   "  current size     = %d\n"
                   "  maximum capacity = %d\n"
                   "  errno            = %d\n"
                   "  data[%d]         = [%p]:\n",
                   this,
                   size_,
                   maxSize_,
                   errNo_,
                   size_,
                   buf_);


    if (buf_ != nullptr) {
        for (int i = 0; i < maxSize_; i++) {
            if (i < size_) {
                fprintf(fdump, "   *[%d] = %d\n", i, buf_[i]);
            } else {
                fprintf(fdump, "    [%d] = %d\n", i, buf_[i]);
            }
        }
    }
    else {
        fprintf(fdump, "   NO DATA\n");
    }

    fprintf(fdump, "}\n");

    fclose (fdump);
}


int main() {
    Vector v(-1);
}