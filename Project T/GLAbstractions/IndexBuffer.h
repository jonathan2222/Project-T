#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return this->count; }

private:
	unsigned int id;
	unsigned int count;
};

#endif