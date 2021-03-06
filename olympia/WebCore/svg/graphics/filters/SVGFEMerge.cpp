/*
    Copyright (C) 2004, 2005, 2006, 2007 Nikolas Zimmermann <zimmermann@kde.org>
                  2004, 2005 Rob Buis <buis@kde.org>
                  2005 Eric Seidel <eric@webkit.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    aint with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#if ENABLE(SVG) && ENABLE(FILTERS)
#include "SVGFEMerge.h"

#include "Filter.h"
#include "GraphicsContext.h"
#include "SVGRenderTreeAsText.h"

namespace WebCore {

FEMerge::FEMerge(const Vector<RefPtr<FilterEffect> >& mergeInputs) 
    : FilterEffect()
    , m_mergeInputs(mergeInputs)
{
}

PassRefPtr<FEMerge> FEMerge::create(const Vector<RefPtr<FilterEffect> >& mergeInputs)
{
    return adoptRef(new FEMerge(mergeInputs));
}

const Vector<RefPtr<FilterEffect> >& FEMerge::mergeInputs() const
{
    return m_mergeInputs;
}

void FEMerge::setMergeInputs(const Vector<RefPtr<FilterEffect> >& mergeInputs)
{
    m_mergeInputs = mergeInputs;
}

FloatRect FEMerge::uniteChildEffectSubregions(Filter* filter)
{
    ASSERT(!m_mergeInputs.isEmpty());

    FloatRect uniteEffectRect = m_mergeInputs[0]->calculateEffectRect(filter);

    for (unsigned i = 1; i < m_mergeInputs.size(); i++)
        uniteEffectRect.unite(m_mergeInputs[i]->calculateEffectRect(filter));

    return uniteEffectRect;
}

void FEMerge::apply(Filter* filter)
{
    ASSERT(!m_mergeInputs.isEmpty());

    for (unsigned i = 0; i < m_mergeInputs.size(); i++) {
        m_mergeInputs[i]->apply(filter);
        if (!m_mergeInputs[i]->resultImage())
            return;
    }

    GraphicsContext* filterContext = getEffectContext();
    if (!filterContext)
        return;

    for (unsigned i = 0; i < m_mergeInputs.size(); i++) {
        FloatRect destRect = calculateDrawingRect(m_mergeInputs[i]->scaledSubRegion());
        filterContext->drawImage(m_mergeInputs[i]->resultImage()->image(), DeviceColorSpace, destRect);
    }
}

void FEMerge::dump()
{
}

TextStream& FEMerge::externalRepresentation(TextStream& ts, int indent) const
{
    writeIndent(ts, indent);
    ts << "[feMerge";
    FilterEffect::externalRepresentation(ts);
    ts << " mergeNodes=\"" << m_mergeInputs.size() << "\"]\n";
    if (!m_mergeInputs.isEmpty()) {
        const Vector<RefPtr<FilterEffect> >::const_iterator end = m_mergeInputs.end();
        for (Vector<RefPtr<FilterEffect> >::const_iterator it = m_mergeInputs.begin(); it != end; ++it)
            (*it)->externalRepresentation(ts, indent + 1);
    }
    return ts;
}

} // namespace WebCore

#endif // ENABLE(SVG) && ENABLE(FILTERS)
