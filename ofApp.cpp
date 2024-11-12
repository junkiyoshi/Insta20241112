#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(1);

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = this->location_list.size() - 1; i >= 0; i--) {

		this->radius_list[i] += this->speed_list[i];

		if (this->radius_list[i] > this->max_radius_list[i]) {

			this->location_list.erase(this->location_list.begin() + i);
			this->radius_list.erase(this->radius_list.begin() + i);
			this->speed_list.erase(this->speed_list.begin() + i);
			this->max_radius_list.erase(this->max_radius_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}
	}

	ofColor color;
	for (int i = 0; i < 3; i++) {

		int random_deg = (ofGetFrameNum() * 4) + ofRandom(-5, 5) + i * 120;
		int random_radius = 100 + (ofGetFrameNum() * 3) % 400 + ofRandom(-30, 30);

		auto location = glm::vec2(random_radius * cos(random_deg * DEG_TO_RAD), random_radius * sin(random_deg * DEG_TO_RAD));
		this->location_list.push_back(location);
		this->radius_list.push_back(1);
		this->speed_list.push_back(ofRandom(0.2, 0.5));
		this->max_radius_list.push_back(ofRandom(15, 30));
		color.setHsb(ofRandom(255), 255, 255);
		this->color_list.push_back(color);
	}

	this->line.clear();
	for (int i = 0; i < this->location_list.size(); i++) {

		this->line.addVertex(glm::vec3(this->location_list[i], 0));

		auto alpha = this->radius_list[i] < this->max_radius_list[i] * 0.5 ? 255 : ofMap(this->radius_list[i], this->max_radius_list[i] * 0.5, this->max_radius_list[i], 255, 0);
		this->line.addColor(ofColor(this->color_list[i], alpha));
		this->color_list[i] = ofColor(this->color_list[i], alpha);
	}

	for (int i = 0; i < this->line.getNumVertices(); i++) {

		for (int k = i + 1; k < this->line.getNumVertices(); k++) {

			if (this->radius_list[i] > 2 && this->radius_list[k] > 2) {

				auto distance = glm::distance(this->line.getVertex(i), this->line.getVertex(k));

				if (distance < 40) {

					this->line.addIndex(i);
					this->line.addIndex(k);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	this->line.drawWireframe();
	for (int i = 0; i < this->location_list.size(); i++) {

		ofPushMatrix();
		ofTranslate(this->location_list[i]);

		ofSetColor(ofColor(0, this->color_list[i].a));

		ofNoFill();
		ofDrawCircle(glm::vec2(), this->radius_list[i]);

		ofFill();
		ofDrawCircle(glm::vec2(), 2);

		ofPopMatrix();
	}

	/*
	int start = 50;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}