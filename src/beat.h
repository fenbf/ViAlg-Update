#pragma once

// simple and smal class CBeat ------------------------------------------------+
// create CBeat object, initialise it with a tempo and run Check method to
// verify the beat - if it is true perform action
class CBeat {
public:
	CBeat(): m_iT1(0), m_iT2(0), m_fTempo(0.0) { }
	CBeat(double fTempo) { SetTempoBPS(fTempo); }
	~CBeat() { }

	bool Check(double fTime) { // usually call Check(g_Timer.getTime());
		m_iT2 = m_iT1; 
		m_iT1 = (unsigned long)(fTime*m_fTempo); 
		return (m_iT1 > m_iT2); 
	}

	void SetTempoBPS(double fTempo) { m_fTempo = fTempo; }
	void SetTempoBPM(double fTempo) { m_fTempo = fTempo/60.0; }
	double GetTempoBPS() { return m_fTempo; }
	double GetTempoBPM() { return m_fTempo*60.0; }	

private:
	unsigned long m_iT1, m_iT2;
	double m_fTempo;
};

