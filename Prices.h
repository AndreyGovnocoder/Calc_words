#pragma once

class Prices
{
public:
	Prices(int id, const float& acrylic, const float& frezer, const float& pvc);
	Prices() = default;
	~Prices() = default;

	void set_id(int id) { _id = id; }
	void set_acrylic(const float& acrylic) { _acrylic = acrylic; }
	void set_frezer(const float& frezer) { _frezer = frezer; }
	void set_pvc(const float& pvc) { _pvc = pvc; }

	int get_id() { return _id; }
	const float& get_acrylic() const { return _acrylic; }
	const float& get_frezer() const { return _frezer; }
	const float& get_pvc() const { return _pvc; }

private:
	int _id;
	float _acrylic;
	float _frezer;
	float _pvc;
};