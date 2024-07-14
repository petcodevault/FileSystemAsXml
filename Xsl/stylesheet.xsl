<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE xsl:stylesheet [
  <!ENTITY nbsp "&#160;">
]>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                version="1.0">

  <!-- Define your variable with the result tree fragment -->
  <xsl:variable name="iconDataUriMap">
    <data extension="pdf">iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAACXBIWXMAAADsAAAA7AF5KHG9AAAAGXRFWHRTb2Z0d2FyZQB3d3cuaW5rc2NhcGUub3Jnm+48GgAAAxlJREFUWIXFl11oHFUYhp/vzGzYiMkmovlBRVHTpP6ggjQUq6hVK2p/olCLIorYXgkJgqSK2F5ZvRGKP0FTrZZqEYq2WzS0goVKbbWk7YWgMTFEpbZqxe5Woc7OzOvFEKzY3TTjZvPCXMw5h+954PvmwBin5a8GupxjsYxLEBmqGaNk4vsoJp89ycg/y4AgW8rxMvAY4KoK/m9iYEOmQK/BKRO4oIntJu6dYfC/YiLvF+mxoJGHMTbVEj4ZiYccxuOzAQcwY6UFOYpAwyw5FFxauOtZjrdmHdbR+X8Ecqkm3m7oxq1ZB23t+Dt2Q7Y+tUE6ge4b0cfbiQdfhbZ27OZbayuABBLW0pYInXd+bQU0NoJ1dMGcruT96JHUAn4qgeEvsQ3v4erqkoUfJlILpGvBb8fR/r3YLbfDsaNoYrzGAoC++xaAePeuZCZqKuD7uJ4HALDW9tTw1AJ2253Q1g4ni9j8BbgHH00tkGoI3SMrAYjf3YhGR/BeGkCj36AD+6ddy4Ic02qgdc7F3/cVmBHOm5sIDLyNW3gX0drV0NKKNTUDoGM/ocPD6IvPy87JtFvg+vrBOTQ+ht29FH/rEG7J/dDSivfaRqyjEx3/FRULWEcX/rZP8N7cUr5gkENn+5QWXCcFgSRJYah432cKn39OpWV3KLg4p3DtaumXnxU+86SCyy9Q0OwUHzwgScn7GWqelUDppusVD+2QokiSFO/8qGzBUvdVirZskk78Lv35R3J+z6cKmt0Zz1eegWw93ovrsUX3oPc341Y9AXFMOO9KdOTHyr3KZLDLroBSCY2PlW9p2Z2mZvxdeyGTIZx/DXb1tVB/DlF/79RwSMAjX1eEQ4XP0HvqWXR4mKh3Fd4L67GFi4jfGSTe/NbU8Gmk8j3Q0oq/dSiBv/EK0dN9VYVDpXvg3Aa8vn648CKU/4B4KF91+KRAAWickepT54QDDs0SHOCQkxicLbrEoAlc2MiHMpbUEm6wzS9wnzOI/SLLgddJfhxnOjEw4BdYYSA7fedUA3M8j8WCSxF1VcUagcFEFJPPFhmdXP4btuCEKGa3Ru0AAAAASUVORK5CYII=</data>
    
    <data extension="djvu">iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAACXBIWXMAAADsAAAA7AF5KHG9AAAAGXRFWHRTb2Z0d2FyZQB3d3cuaW5rc2NhcGUub3Jnm+48GgAAAvZJREFUWIXtlUtoE0EYgL+kadK1Jm5jq1ipBFtRSz34goKvIlVQVHygHhRRPFgPRUTw3Jt66UXwIr15avGBB0WNULFSsa2IVCMIUtnWUGNsujGb3SSb9VATaptN1jTqQb/T7Mzs/387888s/OcvY8s3KO7vEBOGdh7A5Ux3TvRcmfxjAuL+DjGBNggEwABsq13l6fWllrCbDSSM+FEDo1+5c2mvcufyXsMwnmsp+5FSJoccKxCJRM4CntNXb7UCdLUf9APcfDbcCnBoU5N/+vxwOPysvr6+r1gBR46+awCNdYsyz5nEPz1nUFU1JUlSa11d3ZNiBEy3IEPXo0G6Hg2ajuu67pBl2S9J0rbfIhCJqURiat45c5EoKGCVYiVMBZyOsuzXOx1lv00iVxECsGfjKo53dmPDxo0Lh00DVFVVoev6zJg3gWorArmOoWHlxUKIopj3ls1Qshoolr8uYLoFO56MMDCR//gVQR8nm7dM7zAtwoEJlUaPi52L5xNJ6twak5GT6Wxfhofj3wCw22B4UqNpgYv0tCp6K2s0elyZ9uaZeUwFAPbVummpmcdHJcm5FV629Y4AIDqndq7ZK1BRZmMklmD3EjcnXoxxcWU194JRfJXOrMC+Wne2PZOCNdAbUjgzFGTga5wDSz28lTU63oToeBOiN6QA4P+s0FJTidthp6WmkseflUJhrQtk+BBLsqQi94J90VIEZJWLq6oJyCohLVV6geWV5QTVFKd8Iu0NXgBcdhuJHxv+YDxG2/IqHozHLCe3JNC8UOD6+lo2eAVuj8kMTai0N3i5tGYxx5YtyBbh/WCUcruNe8EoAGPxJGvFCtwOO+vECkbjyV8XuPspyquISl9YoaV3BDmZ5vWkyqF+CUlJcLBfYnhyqrAC0QRtQ594F00A0D0aRU0bvNvVgKIb9IxGc+YwvQe2+9/z8ps++4258ZSTzVstCcTjcTRt9rGxis/ns/QvML0HBEFAEISiBaySqwbCJYhbihj/+Uf4Dhj5RnFHVhrLAAAAAElFTkSuQmCC</data>

    <data extension="zip">iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAA7AAAAOwBeShxvQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAAJwSURBVFiF7ZVNSFRRFMd/9808J5s0Rcd0YSMV9IUIfYiOTasMKsNFC5FAol2bdiUEYRYtgjYtohIiJAkpkiSsNi6itFwEWVabpxYVYlY05AdMT2+Lm2XzHjN3ZKZp4R8e73Leuf/ze+e8xxW4SN4OlmByFkkdEHDLccjwSQq2HRehG+e18n9JOIr3lBVjywFgdTJGeJZBYMcc0amjItx5UXeb4YjYtDqKl5+C6g51Fe+K72f6L8j+g4cXD4Dc5wj5iiCvHAqrwL8GzJUgPO6OAg+G77J81NioA+AcQXdwGsh2za65CQWVav2yBUbb/zxTI1joFGU22iBCHXfiAbh0wEU565W5mQuTwzDxGL5bCTaJLDxZnbK/oS5uVmzAtQPhLsjfotYvTsLb604nRwd+V5jCtutFqL3XDUCvA1YbvD4H0++hZDdsaobcDVpbkfgxvF1yoKl68QBjD8C6BDNjEAjDuiMQqNEDABDkgvee7Du0PfaRV8tg4zHIq1BvPXINxnshMvR3zpwNkTfxXPJg9hZQljxA/lb1CwJMjqiPMFbShpmPiZyKYgN6I3jaBPcr4Osz2HwC9gxC6QGtrYmk14G5qLq+DYKRpWL25D8EmNfQ6ZQUXSi9EQD4g7DfgnB3hgA82SC8YOZkCGB+9oaZIYCineq+vBRWrE0ZgP5HuKoW+hqgMATFtWANpwQgiRGYMP0BfkTUwZMi6Xfg1RmovKLOg+fNGQD4/AQexj3aFyX9EaRJSwBLAP8lwJc01ptIDCBkTxoB7iYGkJ4WJO/SUHwU02xNCCDqR8cxRRVwFfiUgsLjINqY9VaJvZZjBD8BkoqagQikcmYAAAAASUVORK5CYII=</data>

    <default>iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAABHNCSVQICAgIfAhkiAAAAAlwSFlzAAAA7AAAAOwBeShxvQAAABl0RVh0U29mdHdhcmUAd3d3Lmlua3NjYXBlLm9yZ5vuPBoAAADySURBVFiF7dcxSgNRFIXhT4Wx1NrCFCK4iEiCWxBcgjsQscteLMQlKCksLCLYWbkE3cEEJBaTgL5MzGReAhb3h9cc3tz7w0wxh3k6GKLEpOV5R7dmdiOGuEHR8vkBHvGBfpsBZcbymcBgunypxHZNVmCcITDjCee4x9kqAuvkeSpx95dEyiRz6SVuk6yPT5yml7cWCNTlTdnHK0Z4+5F3cYyTTQvAHi5wlOTXTWbnvoKVZm/6I1xKCIRACIRACIRACIRACPxLgbG8araIXVXt+8VOzcUeDvCCrzUtL3Cl+iVPS8scHVW7zann6SnxgMN02Ter0UNOfhP2XAAAAABJRU5ErkJggg==</default>    

    <!-- Add more data elements for other file types -->
  </xsl:variable>

  <!-- Шаблон матчинга корневого элемента -->
  <xsl:template match="/filesystem">
    <html>
      <head>
        <style>
          table {
            width: 100%;
            border-collapse: collapse;
          }
          th, td {
            border: 1px solid black;
            padding: 8px;
            text-align: left;
          }
          th {
            background-color: #f2f2f2;
          }
        </style>
      </head>
      <body>
        <!-- Передаем полный путь к текущему каталогу в качестве названия таблицы -->
        <xsl:apply-templates select="directory"/>
      </body>
    </html>
  </xsl:template>

  <!-- Шаблон матчинга элемента <directory> -->
  <xsl:template match="directory">
    <!-- Строим полный путь к текущему каталогу -->
    <xsl:variable name="directoryPath">
      <xsl:for-each select="ancestor-or-self::directory">
        <xsl:value-of select="@name"/>
        <xsl:if test="position() != last()">::</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- Проверяем, есть ли файлы в текущем каталоге -->
    <xsl:if test="file">
      <h2><xsl:value-of select="$directoryPath"/></h2>
      <table>
        <tr>
          <th>Name</th>
          <th>Size</th>
          <th>Last Modified</th>
          <th>SHA-1</th>
        </tr>
        <xsl:apply-templates select="file"/>
      </table>
    </xsl:if>
    <!-- Применяем шаблоны только для подкаталогов -->
    <xsl:apply-templates select="directory"/>
  </xsl:template>

  <!-- Шаблон матчинга элемента <file> -->
  <xsl:template match="file">

    <!-- Extract the file extension using a recursive template -->
    <xsl:variable name="extension">
      <xsl:call-template name="get-extension">
        <xsl:with-param name="filename" select="@name"/>
      </xsl:call-template>
    </xsl:variable>

    <tr>
      <td>
         <img src="data:image/png;base64,{msxsl:node-set($iconDataUriMap)/data[@extension=$extension] | msxsl:node-set($iconDataUriMap)/default}" alt="Icon"/>&nbsp;
         <xsl:value-of select="@name"/>
      </td>
      <!-- Преобразование размера файла -->
      <td>
        <xsl:choose>
          <xsl:when test="@size &gt;= 1073741824">
            <xsl:value-of select="format-number(@size div 1073741824, '0.##')"/> Гб
          </xsl:when>
          <xsl:when test="@size &gt;= 1048576">
            <xsl:value-of select="format-number(@size div 1048576, '0.##')"/> Мб
          </xsl:when>
          <xsl:when test="@size &gt;= 1024">
            <xsl:value-of select="format-number(@size div 1024, '0.##')"/> Кб
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="@size"/> байт
          </xsl:otherwise>
        </xsl:choose>
      </td>
      <td><xsl:value-of select="@lastModified"/></td>
      <td><xsl:value-of select="@hash"/></td>
    </tr>
  </xsl:template>

  <!-- Recursive template to get the extension -->
  <xsl:template name="get-extension">
    <xsl:param name="filename"/>
    <xsl:choose>
      <xsl:when test="contains($filename, '.')">
        <xsl:call-template name="get-extension">
          <xsl:with-param name="filename" select="substring-after($filename, '.')"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$filename"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>