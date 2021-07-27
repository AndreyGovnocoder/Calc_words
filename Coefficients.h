#pragma once

class Coefficients
{
public:
	Coefficients() = default;
	~Coefficients() = default;

	void set_id(const int id) { _id = id; }
	void set_netProfit(const float& netProfit) { _netProfit = netProfit; }
	void set_laborCosts(const float& laborCosts) { _laborCosts = laborCosts; }
	void set_height_5_11(const float& height_5_11) { _height_5_11 = height_5_11; }
	void set_height_12_19(const float& height_12_19) { _height_12_19 = height_12_19; }
	void set_height_20_29(const float& height_20_29) { _height_20_29 = height_20_29; }
	void set_height_30_49(const float& height_30_49) { _height_30_49 = height_30_49; }
	void set_height_more50(const float& height_more50) { _height_more50 = height_more50; }
	void set_advertAgency(const float& advertAgency) { _advertAgency = advertAgency; }
	void set_otherClient(const float& otherClient) { _otherClient = otherClient; }

	int get_id() const { return _id; }
	const float& get_netProfit() const { return _netProfit; }
	const float& get_laborCosts() const { return _laborCosts; }
	const float& get_height_5_11() const { return _height_5_11; }
	const float& get_height_12_19() const { return _height_12_19; }
	const float& get_height_20_29() const { return _height_20_29; }
	const float& get_height_30_49() const { return _height_30_49; }
	const float& get_height_more50() const { return _height_more50; }
	const float& get_advertAgency() const { return _advertAgency; }
	const float& get_otherClient() const { return _otherClient; }

private:
	int _id;
	float _netProfit;
	float _laborCosts;
	float _height_5_11;
	float _height_12_19;
	float _height_20_29;
	float _height_30_49;
	float _height_more50;
	float _advertAgency;
	float _otherClient;
};