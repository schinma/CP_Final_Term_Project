#pragma once

class Viewer;

/**
 * \brief A class to control the viewing parameters.
 *
 * This class maintains and provides access to parameters of a simple viewing model,
 * and it supports basic viewing operations like translation, rotation, and zooming.
 * The view parameters are:
 *		- The view point (where the camera is located)
 *		- The view center (a point that is being looked at; the closer it is to the
 *		   view point, the greater the degree of zooming)
 *		- The up vector (defines the global vertical axis; typically this is the y axis)
 *		- The field of view (defined as the vertical angular span of the viewing
 *		   frustrum in radians
 *		- The aspect ratio (the ratio width/height for the resultant image)
*/

#include <glm/glm.hpp>


class Viewer {
public:

	/** Constructor */
	Viewer(
		const glm::vec3 &viewPoint, const  glm::vec3 &viewCenter, const  glm::vec3 &upVector,
		float fieldOfView, float aspectRatio
	);

	/** The worldspace location of the center of focus */
	glm::vec3 getViewPoint() const;
	/** This corresponds to the worldspace location of  OpenGL's "look at" point */
	glm::vec3 getViewCenter() const;
	/**	The up vector in worldspace coordinates */
	glm::vec3 getUpVector() const;
	float getFieldOfView() const;
	float getAspectRatio() const;

	/** The (normalized) worldspace vector from the viewpoint	to the view center */
	glm::vec3 getViewDir() const;

	glm::vec3 getImagePlaneHorizDir() const;

	glm::vec3 getImagePlaneVertDir() const;

	void translate(float changeHoriz, float changeVert, bool parallelToViewPlane);

	void zoom(float changeVert);

	void rotate(float changeHoriz, float changeVert);

	void centerAt(const  glm::vec3 &pos);


	void lookFrom(const  glm::vec3 &pos);

	/** Set the field of view*/
	void setFieldOfView(float fieldOfView);
	/** Set up the aspect ratio*/
	void setAspectRatio(float aspectRatio);
	/** Set up the translate speed */
	void setTranslateSpeed(float translateSpeed);
	/** Set up the zoom factor*/
	void setZoomFraction(float zoomFraction);
	/** Set up the roation speed */
	void setRotateSpeed(float rotateSpeed);

private:

	glm::vec3 m_viewPoint;
	glm::vec3 m_viewCenter;
	glm::vec3 m_upVector;
	float m_fieldOfView;
	float m_aspectRatio;

	float m_translateSpeed;
	float m_zoomFraction;
	float m_rotateSpeed;

	glm::vec3 m_viewDir;
	glm::vec3 m_imagePlaneHorizDir;
	glm::vec3 m_imagePlaneVertDir;


	float m_displayWidth;
	float m_displayHeight;

	
	float m_lastDesired[3];


	void getFrustrumInfo();
};