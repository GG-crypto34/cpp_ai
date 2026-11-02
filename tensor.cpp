#include "tensor.h"

const std::vector<float>& Tensor::GetData () const
{
    return Data_;    
};
const std::vector<size_t>& Tensor::GetShape() const
{
    return Shape_;
};
const std::vector<size_t>& Tensor::GetStrides() const
{
    return Strides_;
};

void Tensor::SetData(std::vector<float> data)
{
    size_t sizeOfTensor = 1;
    for(size_t i = 0; i < Shape_.size(); i++) 
    {
        sizeOfTensor = sizeOfTensor * Shape_[i];
    };
    if(data.size() == sizeOfTensor){ Data_ = data;}
    else{throw "incorrect shape or data of tensor"; };
};

void Tensor::Reshape(std::vector<size_t> shape)
{
    size_t sizeOfTensor = 1;
    ContiguousCheck();
    for(size_t i = 0; i < shape.size(); i++) 
    {
        sizeOfTensor = sizeOfTensor * shape[i];
    };
    if(sizeOfTensor == Data_.size() && IsNonContiguous_)
    {
        Shape_ = shape;
        calculateStrides();
    }
    else
    {throw "Invalid size of tensor or tensor is transposed";};
};

const float& Tensor::GetDataAt(std::vector<size_t> index) const
{
    if(index.size() != Shape_.size()){throw "Invalid index";};
    for(size_t i = 0; i < index.size(); i++)
    {
        if(index[i] >= Shape_[i]){throw "Invalid index";}
    };
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides_.at(i);
    };
    return Data_.at(findex);
};

void Tensor::EditOnIndex(std::vector<size_t> index, float value)
{
    if(index.size() != Shape_.size()){throw "Invalid index";};
    for(size_t i = 0; i < index.size(); i++)
    {
        if(index[i] >= Shape_[i]){throw "Invalid index";}
    };
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides_.at(i);
    };
    Data_.at(findex) = value;
};

void Tensor::Transpose(std::vector<size_t> permutations)
{
    if(permutations.size() != Shape_.size()){throw "Invalid argument";}
    std::vector<int> seen(permutations.size(), 0);
    for(size_t i = 0; i < permutations.size(); i++)
    {
        seen.at(permutations.at(i)) += 1;
    };
    for(size_t i = 0; i < permutations.size(); i++)
    {
        if(seen.at(i) > 1){throw "Invalid argument";}
    };
    std::vector<size_t> fShape(permutations.size(), 0);
    std::vector<size_t> fStrides(permutations.size(), 0);
    for(size_t i = 0; i < permutations.size(); i++)
    {
        fShape.at(i) = Shape_.at(permutations.at(i));
        fStrides.at(i) = Strides_.at(permutations.at(i));
    };
    Shape_ = fShape;
    Strides_ = fStrides;
    IsNonContiguous_ = false;
};

void Tensor::DangerSetData(std::vector<float> data)
{
    Data_ = data;
};

void Tensor::DangerReshape(std::vector<size_t> shape)
{
    Shape_ = shape;
    calculateStrides();
};

const float& Tensor::DangerGetDataAt(std::vector<size_t> index) const
{
    size_t findex = GetFlatIndex(index);
    return Data_.at(findex);
};

void Tensor::DangerEditOnIndex(std::vector<size_t> index, float value)
{
    size_t findex = GetFlatIndex(index);
    Data_.at(findex) = value;
};

void Tensor::DangerTranspose(std::vector<size_t> permutations)
{
    std::vector<size_t> fShape(permutations.size(), 0);
    std::vector<size_t> fStrides(permutations.size(), 0);
    for(size_t i = 0; i < permutations.size(); i++)
    {
        fShape.at(i) = Shape_.at(permutations.at(i));
        fStrides.at(i) = Strides_.at(permutations.at(i));
    };
    Shape_ = fShape;
    Strides_ = fStrides;
    IsNonContiguous_ = false;
};

const size_t& Tensor::GetFlatIndex(std::vector<size_t> index) const
{
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides_.at(i);
    };
    return findex;
};