#pragma once


struct Position
{
	float x;
	float y;
	void setPosition(float X, float Y)
	{
		x = X;
		y = Y;
	}
};

struct ColorRGBA8
{
	ColorRGBA8() : ColorRGBA8(0, 0, 0, 255) {}
	ColorRGBA8(unsigned char R, unsigned char G, unsigned char B, unsigned char A) :
		r(R), g(G), b(B), alpha(A) {}
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char alpha;
	void setColorRGBA8(unsigned char R, unsigned char G, unsigned char B, unsigned char a){
		r = R;
		g = G;
		b = B;
		alpha = a;
	}
};

struct UV {
	float u;
	float v;
	void setUV(float U, float V){
		u = U;
		v = V;
	}
};

struct Vertex
{
	Position position;

	ColorRGBA8 color;

	UV uv;
	void setColor(unsigned char R, unsigned char G, unsigned char B, unsigned char a)
	{
		color.setColorRGBA8(R, G, B, a);
	}
	void setUV(float U, float V){
		uv.setUV(U, V);
	}
	void setPosition(float X, float Y)
	{
		position.setPosition(X, Y);
	}
};
