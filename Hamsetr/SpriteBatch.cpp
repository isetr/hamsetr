#include "SpriteBatch.h"

#include <algorithm>

namespace Hamsetr {

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) :
		texture(Texture),
		depth(Depth) {

		topLeft.color = color;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
	}

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle) :
		texture(Texture),
		depth(Depth) {

		glm::vec2 halfDimns(destRect.z / 2.0f, destRect.w / 2.0f);

		glm::vec2 tl(-halfDimns.x, halfDimns.y);
		glm::vec2 bl(-halfDimns.x, -halfDimns.y);
		glm::vec2 br(halfDimns.x, -halfDimns.y);
		glm::vec2 tr(halfDimns.x, halfDimns.y);

		tl = rotatePoint(tl, angle) + halfDimns;
		bl = rotatePoint(bl, angle) + halfDimns;
		tr = rotatePoint(tr, angle) + halfDimns;
		br = rotatePoint(br, angle) + halfDimns;

		topLeft.color = color;
		topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
	}

	glm::vec2 Glyph::rotatePoint(glm::vec2& point, float angle) {
		glm::vec2 newPoint;
		newPoint.x = point.x * cos(angle) - point.y * sin(angle);
		newPoint.y = point.x * sin(angle) + point.y * cos(angle);
		return newPoint;
	}

	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init() {
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType) {
		_sortType = sortType;
		_renderBatches.clear();

		_glyphs.clear();
	}

	void SpriteBatch::end() {
		_glyphPointers.resize(_glyphs.size());
		for (int i = 0; i < _glyphs.size(); i++) {
			_glyphPointers[i] = &_glyphs[i];
		}

		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color) {
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle) {
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir) {
		const glm::vec2 right(1.0f, 0.0f);
		float angle = acos(glm::dot(right, dir));
		if (dir.y < 0.0f) angle = -angle;

		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::renderBatch() {
		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); i++) {
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertecies);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches() {
		std::vector<Vertex> vertecies;
		vertecies.resize(_glyphPointers.size()*6);

		if (_glyphPointers.empty()) return;

		int offset = 0;
		int currentVertex = 0;
		_renderBatches.emplace_back(0, 6, _glyphPointers[0]->texture);

		vertecies[currentVertex++] = _glyphPointers[0]->topLeft;
		vertecies[currentVertex++] = _glyphPointers[0]->bottomLeft;
		vertecies[currentVertex++] = _glyphPointers[0]->bottomRight;
		vertecies[currentVertex++] = _glyphPointers[0]->bottomRight;
		vertecies[currentVertex++] = _glyphPointers[0]->topRight;
		vertecies[currentVertex++] = _glyphPointers[0]->topLeft;
		offset += 6;

		for (int currentGlyph = 1; currentGlyph < _glyphPointers.size(); currentGlyph++) {

			if (_glyphPointers[currentGlyph]->texture != _glyphPointers[currentGlyph - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphPointers[currentGlyph]->texture);
			} else {
				_renderBatches.back().numVertecies += 6;
			}
			
			vertecies[currentVertex++] = _glyphPointers[currentGlyph]->topLeft;
			vertecies[currentVertex++] = _glyphPointers[currentGlyph]->bottomLeft;
			vertecies[currentVertex++] = _glyphPointers[currentGlyph]->bottomRight;
			vertecies[currentVertex++] = _glyphPointers[currentGlyph]->bottomRight;
			vertecies[currentVertex++] = _glyphPointers[currentGlyph]->topRight;
			vertecies[currentVertex++] = _glyphPointers[currentGlyph]->topLeft;

			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, vertecies.size()*sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertecies.size() * sizeof(Vertex), vertecies.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void SpriteBatch::createVertexArray() {

		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);

		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs() {
		switch (_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
			break;
		}
		
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return (a->texture < b->texture);
	}
}